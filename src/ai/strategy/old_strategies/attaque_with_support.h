/*
    This file is part of SSL.

    Copyright 2018 Bezamat Jérémy (jeremy.bezamat@gmail.com)

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

#pragma once

#include "strategy.h"
#include <robot_behavior/striker.h>
#include <robot_behavior/robot_follower.h>
#include <robot_behavior/pass.h>
#include <robot_behavior/wait_pass.h>

namespace rhoban_ssl
{
namespace strategy
{
class AttaqueWithSupport : public Strategy
{
private:
  bool behaviors_are_assigned_;
  std::shared_ptr<robot_behavior::RobotFollower> support_;
  std::shared_ptr<robot_behavior::Pass> pass_;
  std::shared_ptr<robot_behavior::Striker> striker_;
  std::pair<rhoban_geometry::Point, double> results_;

public:
  AttaqueWithSupport(ai::AiData& ai_data);
  virtual ~AttaqueWithSupport();

  virtual int minRobots() const;
  virtual int maxRobots() const;
  virtual GoalieNeed needsGoalie() const;

  static const std::string name;

  virtual void start(double time);
  virtual void stop(double time);

  virtual void update(double time);

  virtual void assignBehaviorToRobots(
      std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt);

  virtual std::list<std::pair<rhoban_geometry::Point, ContinuousAngle> >
  getStartingPositions(int number_of_avalaible_robots);
  virtual bool getStartingPositionForGoalie(rhoban_geometry::Point& linear_position, ContinuousAngle& angular_position);

  virtual rhoban_ssl::annotations::Annotations getAnnotations() const;
};

};  // namespace strategy
};  // namespace rhoban_ssl