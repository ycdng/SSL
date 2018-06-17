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

#include "consign_follower.h"

namespace RhobanSSL {
namespace Robot_behavior {

ConsignFollower::ConsignFollower( Ai::AiData & ai_data ):
    RobotBehavior(ai_data)
{ }

ConsignFollower::~ConsignFollower(){ }

void ConsignFollower::avoid_the_ball( bool value ){ }

void ConsignFollower::get_following_position(
        Vector2d * position_to_follow,
        ContinuousAngle * angle
    ) { }

}
}
