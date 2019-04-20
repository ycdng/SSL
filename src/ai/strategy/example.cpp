/*
    This file is part of SSL.

    Copyright 2018 Boussicault Adrien (adrien.boussicault@u-bordeaux.fr)

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

#include "example.h"

#include <robot_behavior/goalie.h>
#include <robot_behavior/striker.h>

namespace rhoban_ssl
{
namespace strategy
{
Example::Example(ai::AiData& ai_data) : Strategy(ai_data)
{
}

Example::~Example()
{
}

/*
 * We define the minimal number of robot in the field.
 * The goalkeeper is not counted.
 */
int Example::minRobots() const
{
  return 1;
}

/*
 * We define the maximal number of robot in the field.
 * The goalkeeper is not counted.
 */
int Example::maxRobots() const
{
  return 1;
}

GoalieNeed Example::needsGoalie() const
{
  return GoalieNeed::YES;
}

const std::string Example::name = "example";

void Example::start(double time)
{
  DEBUG("START PREPARE KICKOFF");
  behaviors_are_assigned_ = false;
}
void Example::stop(double time)
{
  DEBUG("STOP PREPARE KICKOFF");
}

void Example::update(double time)
{
}

void Example::assignBehaviorToRobots(
    std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt)
{
  if (not(behaviors_are_assigned_))
  {
    // We first assign the behhavior of the goalie.
    assign_behavior(getGoalie(), std::shared_ptr<robot_behavior::RobotBehavior>(new robot_behavior::Goalie(ai_data_)));

    // we assign now all the other behavior
    assert(getPlayerIds().size() == 1);
    int id = playerId(0);  // we get the first if in get_player_ids()
    assign_behavior(id, std::shared_ptr<robot_behavior::RobotBehavior>(new robot_behavior::Striker(ai_data_)));

    behaviors_are_assigned_ = true;
  }
}

// We declare here the starting positions that are used to :
//   - place the robot during STOP referee state
//   - to compute the robot order of get_player_ids(),
//     we minimize the distance between
//     the startings points and all the robot position, just
//     before the start() or during the STOP referee state.
std::list<std::pair<rhoban_geometry::Point, ContinuousAngle> >
Example::getStartingPositions(int number_of_avalaible_robots)
{
  assert(minRobots() <= number_of_avalaible_robots);
  assert(maxRobots() == -1 or number_of_avalaible_robots <= maxRobots());

  return { std::pair<rhoban_geometry::Point, ContinuousAngle>(ai_data_.relative2absolute(-1.0 / 3.0, 0.0), 0.0) };
}

//
// This function return false if you don't want to
// give a staring position. So the manager will chose
// a default position for you.
//
bool Example::getStartingPositionForGoalie(rhoban_geometry::Point& linear_position, ContinuousAngle& angular_position)
{
  linear_position = allyGoalCenter();
  angular_position = ContinuousAngle(0.0);
  return true;
}

}  // namespace strategy
}  // namespace rhoban_ssl
