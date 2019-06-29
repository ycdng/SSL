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

#include "pvc_striker_with_support.h"

//#include <robot_behavior/goalie.h>
//#include <robot_behavior/striker.h>
#include <robot_behavior/pvc_robot_follower.h>

namespace rhoban_ssl
{
namespace strategy
{
StrikerWithSupport::StrikerWithSupport() : Strategy()
{
}

StrikerWithSupport::~StrikerWithSupport()
{
}

/*
 * We define the minimal number of robot in the field.
 * The goalkeeper is not counted.
 */
int StrikerWithSupport::minRobots() const
{
  return 3;
}

/*
 * We define the maximal number of robot in the field.
 * The goalkeeper is not counted.
 */
int StrikerWithSupport::maxRobots() const
{
  return 3;
}

GoalieNeed StrikerWithSupport::needsGoalie() const
{
  return GoalieNeed::NO;
}

const std::string StrikerWithSupport::name = "striker_with_support";

void StrikerWithSupport::start(double time)
{
  DEBUG("START PREPARE KICKOFF");
  behaviors_are_assigned_ = false;

  striker_ = std::shared_ptr<robot_behavior::Striker>(new robot_behavior::Striker());
}
void StrikerWithSupport::stop(double time)
{
  DEBUG("STOP PREPARE KICKOFF");
}

void StrikerWithSupport::update(double time)
{
  // std::pair<rhoban_geometry::Point, double> results = GameInformations::find_goal_best_move( ball_position() );
  // rhoban_geometry::Point goal_point = results.first;

  // static_cast<Robot_behavior::Striker*>( striker.get() )->declare_point_to_strik( goal_point );
}

void StrikerWithSupport::assignBehaviorToRobots(
    std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt)
{
  if (not(behaviors_are_assigned_))
  {
    // we assign now all the other behavior
    assert(getPlayerIds().size() == 3);

    assign_behavior(playerId(0), striker_);
    int supportLeft = playerId(1);  // we get the first if in get_player_ids()
    std::shared_ptr<robot_behavior::RobotFollower> support_behaviorL(new robot_behavior::RobotFollower());
    support_behaviorL->declareRobotToFollow(playerId(0), Vector2d(1, 0.5), Ally);
    assign_behavior(supportLeft, support_behaviorL);

    int supportRight = playerId(2);  // we get the first if in get_player_ids()
    std::shared_ptr<robot_behavior::RobotFollower> support_behaviorR(new robot_behavior::RobotFollower());
    support_behaviorR->declareRobotToFollow(playerId(0), Vector2d(1, -0.5), Ally);
    assign_behavior(supportRight, support_behaviorR);

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
StrikerWithSupport::getStartingPositions(int number_of_avalaible_robots)
{
  assert(minRobots() <= number_of_avalaible_robots);
  assert(maxRobots() == -1 or number_of_avalaible_robots <= maxRobots());

  return {
    // std::pair<rhoban_geometry::Point,ContinuousAngle>(
    //     ai_data.relative2absolute(-1.0/3.0, 0.0),
    //     0.0
    // )
  };
}

//
// This function return false if you don't want to
// give a staring position. So the manager will chose
// a default position for you.
//
bool StrikerWithSupport::getStartingPositionForGoalie(rhoban_geometry::Point& linear_position,
                                                      ContinuousAngle& angular_position)
{
  linear_position = Data::get()->field.goalCenter(Ally);
  angular_position = ContinuousAngle(0.0);
  return true;
}

rhoban_ssl::annotations::Annotations StrikerWithSupport::getAnnotations() const
{
  rhoban_ssl::annotations::Annotations annotations;

  for (auto it = this->getPlayerIds().begin(); it != this->getPlayerIds().end(); it++)
  {
    const rhoban_geometry::Point& robot_position = getRobot(*it).getMovement().linearPosition(time());
    // annotations.addText("Behaviour: " + this->name, robot_position.getX() + 0.15, robot_position.getY(), "white");
    annotations.addText("Strategy: " + this->name, robot_position.getX() + 0.15, robot_position.getY() + 0.30, "white");
  }
  return annotations;
}

}  // namespace strategy
}  // namespace rhoban_ssl