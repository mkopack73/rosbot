/*
 * Example ROS node that will read data coming off the ultrasonic sensor.
 * If it detects an obstacle closer than 100cm, it will send out a new
 * Twist command to the turtlesim to tell the turtle to randomly
 * turn left or right by 10/distance radians. It will also move forward by
 * 1/200th the distance reading.
 *
 * This demonstrates taking data off a real sensor that is hooked to an
 * Arduino, feeding that data into ROS, and then having a ROS node read that
 * data and do something based on it.
 */


#include <ros/ros.h>
#include <sensor_msgs/Range.h>
#include <geometry_msgs/Twist.h>

#include <ctime> // need this to see the random number generator
#include <cstdlib> // need this for random number gen

// this is the minimum distance to an obstacle before we need to turn (in cm)
#define OBSTACLEMINRANGE 100.0

class AvoidObstacle {

	// set up the publicly accessible stuff
public:
	// constructor
	AvoidObstacle() {
		// seed the random number generator based on the clock
		srand(time(NULL));


		// we'll set up the subscriptions and publications here.

		// this sets up to listen for the Range messages, buffer up to 100 of them, and which function to route them to
		// notice that the topic name MUST match what the Arduino code is set to push them out on!
		range_sub = n.subscribe("/sensors/ultrasonic_range", 100,&AvoidObstacle::readRange, this);

		// this says to publish twist messages to the topic that turtle1 listens to. and we could publish up to 1000 a second
		command_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",1000);
	}

	/*
	 * Ok, this is what gets called every time we receive a Range message.
	 * It will read the message, and then if the range received is < say 100cm,
	 * we will randomly pick a direction and then publish a message to the turtle
	 * to tell it to turn in the picked direction.
	 */
	void readRange(const sensor_msgs::Range &rangemessage) {

		// define the new Twist message
		geometry_msgs::Twist command;

		// see if the reported range is < our minimum distance to obstacles
		if(rangemessage.range < OBSTACLEMINRANGE) {

			// let's put out some debugging info
			ROS_INFO("Obstacle detected!!!");


			// the closer the range, the more we should turn and the slower we should go
			// let's do 10.0/range, so if it's far away, we don't turn much, but as we get
			// shorter range, the number is smaller so the turn is closer to a 60 degree turn

			double turnamount = 10.0/rangemessage.range;

			ROS_INFO("Range= %f, turnamount=%f",rangemessage.range,turnamount);
			// ok, let's get a random number from 0 to 1 integer.
			// if it's 0, turn left 60
			// if it's 1, turn right 60
			// then publish out the Twist message to make the turtle do that
			if(rand()%2 == 0) {
				command.angular.z = turnamount; //60 degrees in radians
			} else {
				command.angular.z = -turnamount; //60 degrees in radians
			}
		}
		// keep moving forward. The farther the obstacle the faster we can go
		// let's say we move the range / 200.0 so if we don't see anything for a long way move 1/10 the distance
		// before we check again. If something is close, we'll turn and go really slow before checking again.

		double distance = rangemessage.range / 200.0;
		command.linear.x = distance;
		ROS_INFO("drive distance=%f", distance);

		// ok, now publish out the new Twist message
		command_pub.publish(command);
	}


	// set up the private  data fields for this class
private:
	//this just sets up a couple variables for the subscriptions and publishers

	// every ROS node has to have a NodeHandle.
	ros::NodeHandle n;
	// this will hold our subscription for Range messages
	ros::Subscriber range_sub;
	// this will hold our publisher for command messages.
	ros::Publisher command_pub;

};




// here's our main program
int main(int argc, char** argv) {
	ros::init(argc, argv, "ranger_demo");

	// instantiate the node
	AvoidObstacle obstacle;

	// ok, start doing stuff
	while(ros::ok()) {
		// this tells the app to start listening for subscription messages
		ros::spin();
	}
	return 0;

}
