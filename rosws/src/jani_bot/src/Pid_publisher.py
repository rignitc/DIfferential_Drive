#! /bin/python3
import rospy
from geometry_msgs.msg import Vector3

class Pid_publisher:
    def __init__(self) :
        #variables
        self.pid_l = Vector3(3,0,0)
        self.pid_r = Vector3(3,0,0)

        #sub and pub
        pid_l_pub = rospy.Publisher('/PID_L',Vector3,queue_size=10)
        pid_r_pub = rospy.Publisher('/PID_R',Vector3,queue_size=10)

        while not rospy.is_shutdown():
            pid_l_pub.publish(self.pid_l)
            pid_r_pub.publish(self.pid_r)

if __name__ == "__main__":
    rospy.init_node("Pid")
    try:
        pid_publ = Pid_publisher()
    except rospy.ROSInterruptException:
        pass

