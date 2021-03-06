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

#pragma once

#include <debug.h>
#include <vector>
#include <math/vector2d.h>

#include <math/curve.h>
#include "pid.h"

namespace robot_control_details
{
struct fct_wrapper
{
  std::function<double(double u)> rotation;

  fct_wrapper(const std::function<double(double u)>& rotation) : rotation(rotation){};

  Vector2d operator()(double t)
  {
    return Vector2d(rotation(t), 0.0);
  };
};
}  // namespace robot_control_details

class CurveForRobot
{
public:
  robot_control_details::fct_wrapper rotation_fct;

  Curve2d translation_curve;
  Curve2d angular_curve;

  // DifferentiableVelocityConsign tranlsation_consign;
  // DifferentiableVelocityConsign angular_consign;
  ContinuousVelocityConsign tranlsation_consign;
  ContinuousVelocityConsign angular_consign;

  double calculus_error_for_translation;
  double calculus_error_for_rotation;

  RenormalizedCurve translation_movment;
  RenormalizedCurve rotation_movment;

public:
  CurveForRobot(const std::function<Vector2d(double u)>& translation, double translation_velocity,
                double translation_acceleration, const std::function<double(double u)>& rotation,
                double angular_velocity, double angular_acceleration, double calculus_step);
  Vector2d translation(double t) const;
  double rotation(double t) const;

  void printTranslationCurve(double dt) const;
  void printTranslationMovment(double dt) const;

  void printRotationCurve(double dt) const;
  void printRotationMovment(double dt) const;
};

class RobotControl
{
public:
  static constexpr double security_margin = .92;

private:
  double translation_velocity_limit_;
  ContinuousAngle rotation_velocity_limit_;
  double translation_acceleration_limit_;
  ContinuousAngle rotation_acceleration_limit_;

public:
  Control limitedControl(const Vector2d& robot_position, const ContinuousAngle& robot_orientation,
                         const Vector2d& robot_linear_velocity, const ContinuousAngle& robot_angular_velocity) const;

  RobotControl();

  void setLimits(double translation_velocity_limit, double rotation_velocity_limit,
                 double translation_acceleration_limit, double rotation_acceleration_limit);

  virtual Control noLimitedControl() const = 0;

  /* TODO : write a documentation to explaint that function */
  virtual double getDt() const = 0;
};

class RobotControlWithPid : public RobotControl, public PidController
{
public:
  virtual Control noLimitedControl() const;

  virtual double getDt() const;
};
