/*
    This file is part of SSL.

    Copyright 2019 RomainPC (romainpc.lechat@laposte.net)

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

#include "see_robot.h"
#include <math/vector2d.h>

namespace rhoban_ssl
{
namespace robot_behavior
{
namespace Beginner
{
See_Robot::See_Robot(ai::AiData& ai_data, int target_id)
  : RobotBehavior(ai_data), follower(Factory::fixed_consign_follower(ai_data))

{
  target_robot_id = target_id;
}

void See_Robot::update(double time, const ai::Robot& robot, const ai::Ball& ball)
{
  // At First, we update time and update potition from the abstract class robot_behavior.
  // DO NOT REMOVE THAT LINE
  RobotBehavior::updateTimeAndPosition(time, robot, ball);

  annotations.clear();

  const rhoban_geometry::Point& robot_position = robot.getMovement().linearPosition(ai_data_.time);

  ContinuousAngle target_rotation = robot.getMovement().angularPosition(ai_data_.time);

  // Condition to check if the target robot is not the robot itself.
  // A robot which try to look itself will do nothing.
  if (target_robot_id != robot.id())
  {
    const rhoban_geometry::Point& target_position =
        getRobot(target_robot_id).getMovement().linearPosition(ai_data_.time);

    Vector2d direction = target_position - robot_position;
    target_rotation = vector2angle(direction);
  }

  follower->set_following_position(robot_position, target_rotation);

  follower->avoid_the_ball(false);
  follower->update(time, robot, ball);
}

void See_Robot::set_robot_id_to_see(int id)
{
  target_robot_id = id;
}

int See_Robot::get_robot_id_to_see() const
{
  return target_robot_id;
}

Control See_Robot::control() const
{
  Control ctrl = follower->control();
  return ctrl;
}

See_Robot::~See_Robot()
{
  delete follower;
}

rhoban_ssl::annotations::Annotations See_Robot::getAnnotations() const
{
  rhoban_ssl::annotations::Annotations annotations;
  annotations.addAnnotations(this->annotations);
  annotations.addAnnotations(follower->getAnnotations());
  return annotations;
}

}  // namespace Beginner
}  // namespace Robot_behavior
}  // namespace rhoban_ssl
