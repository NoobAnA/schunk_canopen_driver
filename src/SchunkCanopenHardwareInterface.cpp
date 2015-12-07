// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Felix Mauch <mauch@fzi.de>
 * \date    2015-12-7
 *
 */
//----------------------------------------------------------------------


#include "schunk_canopen_driver/SchunkCanopenHardwareInterface.h"

#include <icl_hardware_canopen/SchunkPowerBallNode.h>

SchunkCanopenHardwareInterface::SchunkCanopenHardwareInterface (ros::NodeHandle& nh,
                                                                boost::shared_ptr<CanOpenController>& canopen)
  : m_node_handle (nh),
    m_canopen_controller (canopen)
{
  init();
}

void SchunkCanopenHardwareInterface::init()
{
  m_node_ids = m_canopen_controller->getNodeList();
  m_joint_position_commands.resize(m_node_ids.size());
  m_joint_position_commands_last.resize(m_node_ids.size());
  m_joint_positions.resize(m_node_ids.size());
  m_joint_velocity.resize(m_node_ids.size());
  m_joint_effort.resize(m_node_ids.size());
  m_joint_names.clear();
  m_joint_names.push_back("arm_0_joint");
  m_joint_names.push_back("arm_1_joint");
  m_joint_names.push_back("arm_2_joint");
  m_joint_names.push_back("arm_3_joint");
  m_joint_names.push_back("arm_4_joint");
  m_joint_names.push_back("arm_5_joint");

  // Initialize controller
  for (std::size_t i = 0; i < m_node_ids.size(); ++i) {
    ROS_DEBUG_STREAM("Controller Hardware interface: Loading joint with id: " << m_node_ids[i]);

    // Create joint state interface
    m_joint_state_interface.registerHandle(
        hardware_interface::JointStateHandle(m_joint_names[i],
            &m_joint_positions[i], &m_joint_velocity[i], &m_joint_effort[i]));

    // Create position joint interface
    m_position_joint_interface.registerHandle(
        hardware_interface::JointHandle(
            m_joint_state_interface.getHandle(m_joint_names[i]),
            &m_joint_position_commands[i]));
  }
  registerInterface(&m_joint_state_interface); // From RobotHW base class.
  registerInterface(&m_position_joint_interface); // From RobotHW base class.
}

void SchunkCanopenHardwareInterface::read()
{
  m_joint_positions.resize(m_node_ids.size());
  SchunkPowerBallNode::Ptr node;
  for (size_t i = 0; i < m_node_ids.size(); ++i)
  {
    node = m_canopen_controller->getNode<SchunkPowerBallNode>(m_node_ids[i]);
    m_joint_positions[i] = node->getTargetFeedback();
  }
}

void SchunkCanopenHardwareInterface::write()
{
  if (m_node_ids.size() == m_joint_position_commands.size())
  {
    std::stringstream commanded_positions;
    SchunkPowerBallNode::Ptr node;
    for (size_t i = 0; i < m_node_ids.size(); ++i)
    {
      const uint8_t& nr = m_node_ids[i];
      float pos = m_joint_position_commands[i];
      try
      {
        node = m_canopen_controller->getNode<SchunkPowerBallNode>(nr);
      }
      catch (const NotFoundException& e)
      {
        ROS_ERROR_STREAM ("One or more nodes could not be found in the controller. " << e.what());
        return;
      }
      m_canopen_controller->getNode<SchunkPowerBallNode>(nr)->setTarget(pos);
      commanded_positions << pos << " ";
    }
    if (m_joint_position_commands != m_joint_position_commands_last)
    {
      ROS_INFO_STREAM ("Commanded positions: " << commanded_positions.str());
      m_joint_position_commands_last = m_joint_position_commands;
    }
  }
  else
  {
    ROS_ERROR ("Number of known joints and number of commanded joints do not match!");
  }
}

bool SchunkCanopenHardwareInterface::canSwitch(const std::list< hardware_interface::ControllerInfo >& start_list, const std::list< hardware_interface::ControllerInfo >& stop_list) const
{
    return hardware_interface::RobotHW::canSwitch(start_list, stop_list);
}

void SchunkCanopenHardwareInterface::doSwitch(const std::list< hardware_interface::ControllerInfo >& start_list, const std::list< hardware_interface::ControllerInfo >& stop_list)
{
    hardware_interface::RobotHW::doSwitch(start_list, stop_list);
}

