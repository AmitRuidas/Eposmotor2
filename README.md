# Eposmotor2
Creating a flipper base stair clambing robot
1st you clone it own your catkin_ws
2nd go to root and catkin_make(compile)
3rd rosrun pc7(pkg_name) velocity_server
4th rosrun pc7 velocity_controller (put the values as you want ranges between 300-2000) #for velocity
rosrun pc7 position_server
rosrun pc7 position_controller #for position
rosrun pc7 rotation_server
rosrun pc7 velocity_controller #for left and right rotation
