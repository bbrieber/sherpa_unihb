#!/usr/bin/env python

import Xlib.display as display
import Xlib.X as X
import Tkinter as tk
from Tkinter import Label
import atexit
import signal
import sys
import time
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist

@atexit.register
def autorepeat():
    d=display.Display()    
    d.change_keyboard_control(auto_repeat_mode=X.AutoRepeatModeOn)
    print("shuting down")
    x=d.get_keyboard_control()    


    


class SherpaTeleopApp(object):
    def __init__(self, master, **kwargs):
        self.master=master
        self.pub_pose = rospy.Publisher('sherpa_pose_command', String)
        self.pub_move = rospy.Publisher('sherpa_move_command', Twist)
        rospy.init_node('sherpa_teleop')
        w = Label(master,text="use the keys to navigate")
        w.pack()
        master.title("SHERPA Teleop")
        
        self.current_pose="idle"
        self.master.geometry("350x50")
        self.twist = Twist()
        self.twist.linear.x = 0
        self.twist.linear.y = 0
        self.twist.linear.z = 0
        self.twist.angular.x = 0
        self.twist.angular.y = 0
        self.twist.angular.z = 0
        self.dirty = False
        
        
        self.pose="idle"

        master.bind('<FocusIn>',self.onFocus)
        master.bind('<FocusOut>',self.onFocusLost)
        master.bind('<KeyRelease>',self.released)
        master.bind('<KeyPress>',self.pressed)
        
    def onFocus(self,event):
        print('Gained Focus')
        d=display.Display()
        d.change_keyboard_control(auto_repeat_mode=X.AutoRepeatModeOff)
        x=d.get_keyboard_control()
        
            
    def onFocusLost(self,event):
        print('Lost Focus')
        d=display.Display()
        d.change_keyboard_control(auto_repeat_mode=X.AutoRepeatModeOn)
        x=d.get_keyboard_control()
        
    def pressed(self,event):
        self.dirty = False
        if event.keysym=='w':
            print('Stop')
            self.twist.linear.x = 1
            self.dirty=True

        elif event.keysym=='q':
            print('rot left')
            self.twist.angular.z = 1
            self.dirty=True

        elif event.keysym=='e':
            print('rot right')
            self.twist.angular.z = -1
            self.dirty=True

        elif event.keysym=='a':
            print('left')
            self.dirty=True
            self.twist.linear.y = -1

        elif event.keysym=='d':
            print('right')
            self.dirty=True
            self.twist.linear.y = 1

        elif event.keysym=='s':
            print('back')
            self.dirty=True
            self.twist.linear.x = -1
        if((self.twist.linear.x !=0) or (self.twist.linear.y != 0 ) or (self.twist.angular.z !=0)):
            self.change_pose("walk")
            return
        elif event.keysym=='space':
            print('point')
            self.change_pose("point")
        
            

    def released(self,event):
        self.dirty=False
        if event.keysym=='w':
            print('Stop')
            self.dirty=True
            self.twist.linear.x = 0
            
        elif event.keysym=='q':
            print('rot left')
            self.dirty=True
            self.twist.angular.z = 0

        elif event.keysym=='e':
            print('rot right')
            self.dirty=True
            self.twist.angular.z = 0

        elif event.keysym=='a':
            print('left')
            self.twist.linear.y = 0
            self.dirty=True
        elif event.keysym=='d':
            print('right')
            self.twist.linear.y = 0
            self.dirty=True

        elif event.keysym=='s':
            print('back')
            self.twist.linear.x = 0
            self.dirty=True
            
        if((self.twist.linear.x == 0) and (self.twist.linear.y == 0) and (self.twist.angular.z ==0)):
            if(self.current_pose=="walk"):
                self.change_pose("idle")
                

        if event.keysym=='space':
            if(self.current_pose=="point"):
                self.change_pose("idle")
            
    def change_pose(self,name):
        if(self.current_pose!=name):
            self.current_pose=name
            self.pub_pose.publish(String(name))
        #trigger pose change

    def send_changes(self):
        if rospy.is_shutdown():
            sys.exit(0)
        if(self.dirty):
            self.now = rospy.get_time()
            self.pub_move.publish(self.twist)
            self.dirty=False
        self.master.after(200,self.send_changes)

        
            
if __name__ == '__main__':
    try:
        d=display.Display()
        d.change_keyboard_control(auto_repeat_mode=X.AutoRepeatModeOff)
        x=d.get_keyboard_control()
        root=tk.Tk()
        app=SherpaTeleopApp(root)
        app.send_changes()
        root.mainloop()
        d.change_keyboard_control(auto_repeat_mode=X.AutoRepeatModeOn)
    except rospy.RosInterruptException:
        pass
    
  
#signal.signal(signal.SIGINT, signal_handler)
#signal.pause()
