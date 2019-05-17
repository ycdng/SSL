/*
    This file is part of SSL.

    Copyright 2018 Boussicault Adrien (adrien.boussicault@u-bordeaux.fr)
    Copyright 2018 TO COMPLETE -> Gregwar

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

#include <com/ai_commander.h>
#include <vision/ai_vision_client.h>
#include <robot_behavior/robot_behavior.h>
#include <referee/game_state.h>
#include <core/machine_state.h>
#include <manager/manager.h>
#include <annotations/annotations.h>

namespace rhoban_ssl
{
/**
 * @brief The AI class
 */
class AI : public Task
{
public:
  // bool is_in_simulation;
  AI(std::string manager_name, std::string team_name, AICommander* commander, const std::string& config_path);

  bool runTask() override;
  void stop();

  std::vector<std::string> getAvailableManagers();
  void setManager(std::string manager);
  std::shared_ptr<manager::Manager> getManager() const;
  std::shared_ptr<manager::Manager> getManualManager();

  double getCurrentTime();

private:
  bool running_;
  AICommander* commander_;

  std::map<int, std::shared_ptr<robot_behavior::RobotBehavior> > robot_behaviors_;

  void initRobotBehaviors();
  void updateRobots();

  // SharedData shared_data_;

  std::shared_ptr<manager::Manager> strategy_manager_;
  std::shared_ptr<manager::Manager> manual_manager_;

  Control updateRobot(robot_behavior::RobotBehavior& robot_behavior, double time, data::Robot& robot, data::Ball& ball);

  void sendControl(int robot_id, const Control& control);
  void prepareToSendControl(int robot_id, Control& control);

  void limitsVelocity(Control& ctrl) const;

  void preventCollision(int robot_id, Control& ctrl);
  rhoban_ssl::annotations::Annotations getRobotBehaviorAnnotations() const;

public:
  void getAnnotations(rhoban_ssl::annotations::Annotations& annotations) const;
};

/**
 * @brief The TimeSynchronisation class
 */
class RegulateAiLoopPeriod : public Task
{
  // Task interface
public:
  bool runTask();
};

class TimeUpdater : public Task
{
  // Task interface
public:
  bool runTask();
};

};  // namespace rhoban_ssl
