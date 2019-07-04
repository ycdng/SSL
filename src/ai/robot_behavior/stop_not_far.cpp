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

#include "stop_not_far.h"
#include <math/tangents.h>
#include <math/vector2d.h>

namespace rhoban_ssl
{
namespace robot_behavior
{
StopNotFar::StopNotFar() : RobotBehavior(), follower_(Factory::fixedConsignFollower())
{
}

void StopNotFar::update(double time, const data::Robot& robot, const data::Ball& ball)
{
  // At First, we update time and update potition from the abstract class robot_behavior.
  // DO NOT REMOVE THAT LINE
  RobotBehavior::updateTimeAndPosition(time, robot, ball);
  // Now
  //  this->robot_linear_position
  //  this->robot_angular_position
  // are all avalaible

  // int robot_id = 2;
  // const Robots_table & robot_table = ai_data.robots.at(Vision::Ally);
  // const ai::Robot & robot = robot_table.at(robot_id);
  Vector2d vect_ball_goal = Data::get()->field.goalCenter(Ally) - ballPosition();

  double dist_with_victim = 0.6;
  rhoban_geometry::Point target_position =
      rhoban_geometry::Point(ballPosition().x + dist_with_victim * std::cos(vector2angle(vect_ball_goal).value()),
                             ballPosition().y + dist_with_victim * std::sin(vector2angle(vect_ball_goal).value()));

  Vector2d vect_robot_ball = ballPosition() - linearPosition();
  ContinuousAngle target_rotation = vector2angle(vect_robot_ball);

  follower_->setFollowingPosition(target_position, target_rotation.value());
  follower_->update(time, robot, ball);
}

Control StopNotFar::control() const
{
  Control ctrl = follower_->control();
  return ctrl;
}

StopNotFar::~StopNotFar()
{
  delete follower_;
}

rhoban_ssl::annotations::Annotations StopNotFar::getAnnotations() const
{
  return follower_->getAnnotations();
}

}  // namespace robot_behavior
}  // namespace rhoban_ssl
