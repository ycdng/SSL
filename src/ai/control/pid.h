#ifndef __pid__H__
#define __pid__H__

#include <debug.h>
#include <Eigen/Dense>
#include <math/ContinuousAngle.h>

struct PidControl {
    Eigen::Vector2d velocity_translation;
    ContinuousAngle velocity_rotation;

    PidControl();
    PidControl(
        const Eigen::Vector2d & velocity_translation,
        ContinuousAngle velocity_rotation
    );

};

std::ostream& operator << ( std::ostream &, const PidControl& control );

struct PidController {
    double kp_t,ki_t,kd_t;
    double kp_o,ki_o,kd_o;

    bool static_robot;

    double start_time;
    double time;
    double dt;

    void init_time(double start_time, double dt);

    void update(double current_time);
   
    PidController();
    PidController(
        double p_t, double i_t, double d_t, 
        double p_o, double i_o, double d_o 
    );

    void set_orientation_pid( double kp, double ki, double kd );
    void set_translation_pid( double kp, double ki, double kd );

    void set_static(bool value);
    bool is_static() const ;

    double get_dt() const;
    double get_time() const;

    virtual ContinuousAngle goal_orientation( double t ) const =0;
    virtual Eigen::Vector2d goal_position( double t ) const = 0;

    Eigen::Vector2d translation_control_in_absolute_frame(
        const Eigen::Vector2d & robot_position, 
        ContinuousAngle robot_orientation
    ) const;
    double rotation_control_in_absolute_frame(
        const Eigen::Vector2d & robot_position, 
        ContinuousAngle robot_orientation
    ) const;
    
    virtual PidControl absolute_control_in_absolute_frame(
        const Eigen::Vector2d & robot_position, 
        ContinuousAngle robot_orientation
    ) const;
};

#endif
