/*
 * Copyright 2012 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
/* Desc: Sphere shape
 * Author: Nate Koenig
 * Date: 14 Oct 2009
 */
#include "physics/SphereShape.hh"

using namespace gazebo;
using namespace physics;

//////////////////////////////////////////////////
SphereShape::SphereShape(CollisionPtr _parent)
  : Shape(_parent)
{
  this->AddType(Base::SPHERE_SHAPE);
}

//////////////////////////////////////////////////
SphereShape::~SphereShape()
{
}

//////////////////////////////////////////////////
void SphereShape::Init()
{
  this->SetRadius(this->sdf->GetValueDouble("radius"));
}

//////////////////////////////////////////////////
void SphereShape::SetRadius(double _radius)
{
  this->sdf->GetElement("radius")->Set(_radius);
}

//////////////////////////////////////////////////
double SphereShape::GetRadius() const
{
  return this->sdf->GetValueDouble("radius");
}

//////////////////////////////////////////////////
void SphereShape::FillMsg(msgs::Geometry &_msg)
{
  _msg.set_type(msgs::Geometry::SPHERE);
  _msg.mutable_sphere()->set_radius(this->GetRadius());
}

//////////////////////////////////////////////////
void SphereShape::ProcessMsg(const msgs::Geometry &_msg)
{
  this->SetRadius(_msg.sphere().radius());
}
