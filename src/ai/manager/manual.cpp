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

#include "manual.h"

// The different strategies
#include <strategy/tare_and_synchronize.h>
#include <strategy/from_robot_behavior.h>
#include <robot_behavior/example.h>
#include <robot_behavior/position_follower.h>
#include <robot_behavior/tutorials/beginner/goto_ball.h>
#include <robot_behavior/tutorials/beginner/go_corner.h>
#include <robot_behavior/tutorials/beginner/goalie.h>
#include <robot_behavior/tutorials/medium/defender.h>
#include <robot_behavior/tutorials/beginner/see_ball.h>
#include <robot_behavior/tutorials/beginner/see_robot.h>
#include <robot_behavior/tutorials/beginner/robot_near_ball.h>
#include <robot_behavior/tutorials/beginner/robot_have_ball.h>
#include <robot_behavior/tutorials/beginner/annotations_ball_position.h>
#include <robot_behavior/tutorials/medium/striker.h>
#include <robot_behavior/tutorials/medium/prepare_strike.h>

namespace rhoban_ssl
{
namespace manager
{
Manual::Manual(ai::AiData& ai_data)
  : Manager(ai_data)
  , team_color_(ai::Team::Unknown)
  , goal_to_positive_axis_(true)
  , ally_goalie_id_(0)
  , opponent_goalie_id_(0)
{
  changeTeamAndPointOfView(ai_data.team_color, goal_to_positive_axis_);

  registerStrategy("Example", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                  ai_data,
                                  [&](double time, double dt) {
                                    robot_behavior::Example* example = new robot_behavior::Example(ai_data);
                                    return std::shared_ptr<robot_behavior::RobotBehavior>(example);
                                  },
                                  false  // we don't want to define a goal here !
                                  )));
  registerStrategy("Beginner go to ball",
                   std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                       ai_data,
                       [&](double time, double dt) {
                         robot_behavior::Beginner::Goto_ball* beginner_goto_ball =
                             new robot_behavior::Beginner::Goto_ball(ai_data);
                         return std::shared_ptr<robot_behavior::RobotBehavior>(beginner_goto_ball);
                       },
                       false  // we don't want to define a goal here !
                       )));
  registerStrategy("Beginner - Goalie", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                            ai_data,
                                            [&](double time, double dt) {
                                              robot_behavior::beginner::Goalie* goalie =
                                                  new robot_behavior::beginner::Goalie(ai_data);
                                              return std::shared_ptr<robot_behavior::RobotBehavior>(goalie);
                                            },
                                            false  // we don't want to define a goal here !
                                            )));
  registerStrategy("Medium - Defender", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                            ai_data,
                                            [&](double time, double dt) {
                                              robot_behavior::medium::Defender* defender =
                                                  new robot_behavior::medium::Defender(ai_data);
                                              return std::shared_ptr<robot_behavior::RobotBehavior>(defender);
                                            },
                                            false  // we don't want to define a goal here !
                                            )));
  registerStrategy("Beginner Annotations - Ball position",
                   std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                       ai_data,
                       [&](double time, double dt) {
                         robot_behavior::BeginnerAnnotationsBallPosition* ball_position =
                             new robot_behavior::BeginnerAnnotationsBallPosition(ai_data);
                         return std::shared_ptr<robot_behavior::RobotBehavior>(ball_position);
                       },
                       false  // we don't want to define a goal here !
                       )));
  registerStrategy("Beginner - See ball", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                              ai_data,
                                              [&](double time, double dt) {
                                                robot_behavior::beginner::SeeBall* see_ball =
                                                    new robot_behavior::beginner::SeeBall(ai_data);
                                                return std::shared_ptr<robot_behavior::RobotBehavior>(see_ball);
                                              },
                                              false  // we don't want to define a goal here !
                                              )));
  registerStrategy("Beginner - See Robot 3", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                 ai_data,
                                                 [&](double time, double dt) {
                                                   robot_behavior::Beginner::SeeRobot* see_robot =
                                                       new robot_behavior::Beginner::SeeRobot(ai_data);
                                                   see_robot->setRobotIdToSee(3);
                                                   return std::shared_ptr<robot_behavior::RobotBehavior>(see_robot);
                                                 },
                                                 false  // we don't want to define a goal here !
                                                 )));
  registerStrategy("Beginner - Robot near ball", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                     ai_data,
                                                     [&](double time, double dt) {
                                                       robot_behavior::BeginnerRobotNearBall* near_ball =
                                                           new robot_behavior::BeginnerRobotNearBall(ai_data);
                                                       return std::shared_ptr<robot_behavior::RobotBehavior>(near_ball);
                                                     },
                                                     false  // we don't want to define a goal here !
                                                     )));
  registerStrategy("Beginner - Robot have ball", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                                     ai_data,
                                                     [&](double time, double dt) {
                                                       robot_behavior::BeginnerRobotHaveBall* have_ball =
                                                           new robot_behavior::BeginnerRobotHaveBall(ai_data);
                                                       return std::shared_ptr<robot_behavior::RobotBehavior>(have_ball);
                                                     },
                                                     false  // we don't want to define a goal here !
                                                     )));
  registerStrategy("Intermediate Striker", std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                                               ai_data,
                                               [&](double time, double dt) {
                                                 robot_behavior::IntermediateStriker* striker =
                                                     new robot_behavior::IntermediateStriker(ai_data);
                                                 return std::shared_ptr<robot_behavior::RobotBehavior>(striker);
                                               },
                                               false  // we don't want to define a goal here !
                                               )));
  registerStrategy("Intermediate Prepare to strike",
                   std::shared_ptr<strategy::Strategy>(new strategy::FromRobotBehavior(
                       ai_data,
                       [&](double time, double dt) {
                         robot_behavior::IntermediatePrepareStrike* prepare_strike =
                             new robot_behavior::IntermediatePrepareStrike(ai_data);
                         return std::shared_ptr<robot_behavior::RobotBehavior>(prepare_strike);
                       },
                       false  // we don't want to define a goal here !
                       )));
  registerStrategy(strategy::TareAndSynchronize::name,
                   std::shared_ptr<strategy::Strategy>(new strategy::TareAndSynchronize(ai_data)));
}

void Manual::assignPointOfViewAndGoalie()
{
  // DEBUG(team_color);
  // DEBUG(ai::Team::Yellow);
  changeTeamAndPointOfView(team_color_, goal_to_positive_axis_);
}

void Manual::setTeamColor(ai::Team team_color)
{
  this->team_color_ = team_color;
}

void Manual::defineGoalToPositiveAxis(bool value)
{
  this->goal_to_positive_axis_ = value;
}

void Manual::update(double time)
{
  // if( not( get_strategy_<Strategy::Tare_and_synchronize>().is_tared_and_synchronized() ) ){
  //   assign_strategy( Strategy::Tare_and_synchronize::name, time, get_valid_player_ids() );
  // }
  // update_strategies(time);
  updateCurrentStrategies(time);
  assignPointOfViewAndGoalie();
  // if( ! strategy_was_assigned ){
  //    assign_strategy(
  //        "Goalie",
  //        //"Position Follower",
  //        time, get_team_ids()
  //    );
  //    strategy_was_assigned = true;
  //}
}

Manual::~Manual()
{
}

};  // namespace manager
};  // namespace rhoban_ssl
