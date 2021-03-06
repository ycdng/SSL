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

#include "robot_behavior.h"
#include "factory.h"
#include <random>

namespace rhoban_ssl
{
namespace robot_behavior
{
class SearchShootArea : public RobotBehavior
{
private:
  rhoban_geometry::Point p1_;
  rhoban_geometry::Point p2_;
  int obstructed_view_;

  std::default_random_engine generator_;

  double period_;
  double last_time_changement_;

  rhoban_geometry::Point target_position_;

  ConsignFollower* follower_;
  rhoban_ssl::annotations::Annotations annotations_;

public:
  bool well_positioned;

  SearchShootArea();

  virtual void update(double time, const data::Robot& robot, const data::Ball& ball);

  void setPeriod(double period)
  {
    period_ = period;
  }

  void declareArea(rhoban_geometry::Point p1, rhoban_geometry::Point p2);

  virtual Control control() const;
  virtual rhoban_ssl::annotations::Annotations getAnnotations() const;
  virtual ~SearchShootArea();
};

};  // namespace robot_behavior
};  // namespace rhoban_ssl
