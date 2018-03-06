#include "robot_behavior.h"

namespace RhobanSSL {

namespace detail {

double vec2angle( Eigen::Vector2d direction ){
    double norm = direction.norm();
    if( norm == 0.0 ) return 0.0;
    direction /= norm;
    double res = std::acos( direction[0] );
    if( direction[1] <= 0 ) return -res;
    return res;
}

}

Control::Control():
    PidControl(), kick(false), active(true), ignore(false)
{ }


Control::Control(bool kick, bool active, bool ignore):
    kick(kick), active(active), ignore(ignore)
{ }

Control::Control(const PidControl & c):
    PidControl(c), kick(false), active(true), ignore(ignore)
{ }

std::ostream& operator << ( std::ostream & out, const Control& control  ){
    out << "{ctrl : " << static_cast<PidControl>(control)
        << ", kick : " << control.kick << ", acitve : " << control.active << ", ignore : " << control.ignore <<"}";
    return out;
}

Control Control::make_null(){
    return Control(false, true, false);
}

Control Control::make_desactived(){
    return Control(false, false, true);
}

Control Control::make_ignored(){
    return Control(false, false, true);
}







RobotBehavior::RobotBehavior() : birthday(-1.0) { };

double RobotBehavior::age() const { return lastUpdate - birthday; };
bool RobotBehavior::is_born() const { return birthday > 0; };
double RobotBehavior::set_birthday( double birthday ){
    assert( birthday > 0 );
    this->birthday = birthday;
};

void RobotBehavior::update(
    double time, 
    const Ai::Robot & robot, const Ai::Ball & ball
){
    lastUpdate = time;
    this->robot_position = Eigen::Vector2d( 
        robot.get_movement().linear_position(time).getX(), 
        robot.get_movement().linear_position(time).getY()
    );
    const Movement & mov = robot.get_movement();
    this->ball_position = Eigen::Vector2d(
        ball.get_movement().linear_position(time).getX(),
        ball.get_movement().linear_position(time).getY()
    );
    this->robot_orientation = robot.get_movement().angular_position(
        time
    );
};

}