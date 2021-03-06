/*
    This file is part of SSL.

    Copyright 2018 TO COMPLETE

    SSL is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SSL.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "striker.h"
#include <math/tangents.h>
#include <math/vector2d.h>

namespace rhoban_ssl
{
namespace robot_behavior
{
namespace attacker
{
Striker::Striker(rhoban_geometry::Point striking_point)
  : RobotBehavior(), striking_point_(striking_point), follower_(Factory::fixedConsignFollower())
{
  tempo_ = 0.0;
}

void Striker::update(double time, const data::Robot& robot, const data::Ball& ball)
{
  // At First, we update time and update potition from the abstract class robot_behavior.
  // DO NOT REMOVE THAT LINE
  RobotBehavior::updateTimeAndPosition(time, robot, ball);
  // Now
  //  this->robot_linear_position
  //  this->robot_angular_position
  // are all avalaible

  const rhoban_geometry::Point& robot_position = robot.getMovement().linearPosition(time);

  Vector2d ball_striking_vector = striking_point_ - ballPosition();
  Vector2d ball_robot_vector = robot_position - ballPosition();
  double dist_ball_robot = ball_robot_vector.norm();

  if (ball_striking_vector.norm() != 0)
  {
    ball_striking_vector = ball_striking_vector / ball_striking_vector.norm();
  }

  if (ball_robot_vector.norm() != 0)
  {
    ball_robot_vector = ball_robot_vector / ball_robot_vector.norm();
  }

  double target_radius_from_ball;
  double scalar_ball_robot = -scalarProduct(ball_robot_vector, ball_striking_vector);
  if (tempo_ == 0.0)
  {
    target_radius_from_ball = 0.3;
  }

  if (scalar_ball_robot < 0)
  {
    follower_->avoidTheBall(true);
  }
  else
  {
    follower_->avoidTheBall(false);

    if (dist_ball_robot < target_radius_from_ball)
    {
      follower_->avoidOpponent(false);
    }
  }
  // TODO Add hysteresis
  if (dist_ball_robot > target_radius_from_ball)
  {
    follower_->avoidOpponent(true);
  }

  rhoban_geometry::Point target_position = ballPosition() - ball_striking_vector * (target_radius_from_ball);
  double target_rotation = detail::vec2angle(ball_striking_vector);

  double position_margin = 0.05;
  double waiting_time = 3.0;

  if ((Vector2d(target_position - robot_position).norm() < position_margin) && (tempo_ == 0.0))
  {
    tempo_ = time;
  }

  // if( Vector2d(target_position - robot_position).norm() > position_margin ) {
  //     tempo = 0.0;
  // }

  if ((Vector2d(target_position - robot_position).norm() < position_margin) && (tempo_ != 0.0))
  {
    if (std::abs(time - tempo_) >= waiting_time)
    {
      target_radius_from_ball = -0.5;
    }
  }
  follower_->setFollowingPosition(target_position, target_rotation);
  follower_->update(time, robot, ball);
}

Control Striker::control() const
{
  Control ctrl = follower_->control();

  ctrl.kick_power = 1;

  ctrl.charge = true;
  ctrl.kick = true;
  return ctrl;
}

void Striker::declarePointToStrike(rhoban_geometry::Point point)
{
  striking_point_ = point;
}

Striker::~Striker()
{
  delete follower_;
}

rhoban_ssl::annotations::Annotations Striker::getAnnotations() const
{
  return follower_->getAnnotations();
}

}  // namespace attacker
}  // namespace robot_behavior
}  // namespace rhoban_ssl
