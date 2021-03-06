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

#include <rhoban_geometry/point.h>
#include <rhoban_geometry/segment.h>
#include <vector>

namespace rhoban_geometry
{
double distanceFromPointToLine(const rhoban_geometry::Point& point, const rhoban_geometry::Point& point_line_1,
                               const rhoban_geometry::Point& point_line_2);

std::vector<rhoban_geometry::Point> getIntersectionLineWithCircle(const rhoban_geometry::Point& point_line_1,
                                                                  const rhoban_geometry::Point& point_line_2,
                                                                  const rhoban_geometry::Circle& circle);
}  // namespace rhoban_geometry
