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
/* Desc: A ODEHingeJoint
 * Author: Nate Koenig, Andrew Howard
 * Date: 21 May 2003
 */

#include <boost/bind.hpp>

#include "gazebo_config.h"
#include "common/Console.hh"

#include "physics/Link.hh"
#include "physics/ode/ODEHingeJoint.hh"

using namespace gazebo;
using namespace physics;

//////////////////////////////////////////////////
ODEHingeJoint::ODEHingeJoint(dWorldID _worldId, BasePtr _parent)
    : HingeJoint<ODEJoint>(_parent)
{
  this->jointId = dJointCreateHinge(_worldId, NULL);
}

//////////////////////////////////////////////////
ODEHingeJoint::~ODEHingeJoint()
{
  if (this->applyDamping)
    physics::Joint::DisconnectJointUpdate(this->applyDamping);
}

//////////////////////////////////////////////////
void ODEHingeJoint::Load(sdf::ElementPtr _sdf)
{
  HingeJoint<ODEJoint>::Load(_sdf);

  this->SetParam(dParamFMax, 0);
  this->SetForce(0, 0);
}

//////////////////////////////////////////////////
math::Vector3 ODEHingeJoint::GetAnchor(int /*index*/) const
{
  dVector3 result;

  if (this->jointId)
    dJointGetHingeAnchor(this->jointId, result);
  else
    gzerr << "ODE Joint ID is invalid\n";

  return math::Vector3(result[0], result[1], result[2]);
}

//////////////////////////////////////////////////
void ODEHingeJoint::SetAnchor(int /*index*/, const math::Vector3 &_anchor)
{
  if (this->childLink)
    this->childLink->SetEnabled(true);
  if (this->parentLink)
    this->parentLink->SetEnabled(true);

  if (this->jointId)
    dJointSetHingeAnchor(this->jointId, _anchor.x, _anchor.y, _anchor.z);
  else
    gzerr << "ODE Joint ID is invalid\n";
}


//////////////////////////////////////////////////
math::Vector3 ODEHingeJoint::GetGlobalAxis(int /*_index*/) const
{
  dVector3 result;
  if (this->jointId)
    dJointGetHingeAxis(this->jointId, result);
  else
    gzerr << "ODE Joint ID is invalid\n";

  return math::Vector3(result[0], result[1], result[2]);
}

//////////////////////////////////////////////////
void ODEHingeJoint::SetAxis(int /*index*/, const math::Vector3 &_axis)
{
  if (this->childLink)
    this->childLink->SetEnabled(true);
  if (this->parentLink)
    this->parentLink->SetEnabled(true);

  if (this->jointId)
    dJointSetHingeAxis(this->jointId, _axis.x, _axis.y, _axis.z);
  else
    gzerr << "ODE Joint ID is invalid\n";
}

//////////////////////////////////////////////////
math::Angle ODEHingeJoint::GetAngleImpl(int /*index*/) const
{
  math::Angle result;
  if (this->jointId)
    result = dJointGetHingeAngle(this->jointId);
  else
    gzerr << "ODE Joint ID is invalid\n";

  return result;
}

//////////////////////////////////////////////////
double ODEHingeJoint::GetVelocity(int /*index*/) const
{
  double result = 0;

  if (this->jointId)
    result = dJointGetHingeAngleRate(this->jointId);
  else
    gzerr << "ODE Joint ID is invalid\n";

  return result;
}

//////////////////////////////////////////////////
void ODEHingeJoint::SetVelocity(int /*index*/, double _angle)
{
  this->SetParam(dParamVel, _angle);
}

//////////////////////////////////////////////////
void ODEHingeJoint::SetMaxForce(int /*index*/, double _t)
{
  return this->SetParam(dParamFMax, _t);
}

//////////////////////////////////////////////////
double ODEHingeJoint::GetMaxForce(int /*index*/)
{
  return this->GetParam(dParamFMax);
}

//////////////////////////////////////////////////
void ODEHingeJoint::SetForce(int _index, double _effort)
{
  if (_index < 0 || static_cast<unsigned int>(_index) >= this->GetAngleCount())
  {
    gzerr << "Calling ODEHingeJoint::SetForce with an index ["
          << _index << "] out of range\n";
    return;
  }

  // truncating SetForce effort if velocity limit reached.
  if (this->velocityLimit[_index] >= 0)
  {
    if (this->GetVelocity(_index) > this->velocityLimit[_index])
      _effort = _effort > 0 ? 0 : _effort;
    else if (this->GetVelocity(_index) < -this->velocityLimit[_index])
      _effort = _effort < 0 ? 0 : _effort;
  }

  // truncate effort if effortLimit is not negative
  if (this->effortLimit[_index] >= 0)
    _effort = math::clamp(_effort,
      -this->effortLimit[_index], this->effortLimit[_index]);

  ODEJoint::SetForce(_index, _effort);
  if (this->childLink)
    this->childLink->SetEnabled(true);
  if (this->parentLink)
    this->parentLink->SetEnabled(true);

  if (this->jointId)
    dJointAddHingeTorque(this->jointId, _effort);
  else
    gzerr << "ODE Joint ID is invalid\n";
}

//////////////////////////////////////////////////
double ODEHingeJoint::GetParam(int _parameter) const
{
  double result = 0;

  if (this->jointId)
    result = dJointGetHingeParam(this->jointId, _parameter);
  else
    gzerr << "ODE Joint ID is invalid\n";

  return result;
}

//////////////////////////////////////////////////
void ODEHingeJoint::SetParam(int _parameter, double _value)
{
  ODEJoint::SetParam(_parameter, _value);

  if (this->jointId)
    dJointSetHingeParam(this->jointId, _parameter, _value);
  else
    gzerr << "ODE Joint ID is invalid\n";
}
