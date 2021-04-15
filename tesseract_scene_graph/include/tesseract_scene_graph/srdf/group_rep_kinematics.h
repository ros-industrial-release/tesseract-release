#ifndef TESSERACT_SCENE_GRAPH_SRDF_GROUP_REP_KINEMATICS_H
#define TESSERACT_SCENE_GRAPH_SRDF_GROUP_REP_KINEMATICS_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <array>
#include <console_bridge/console.h>
#include <tinyxml2.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_common/utils.h>
#include <tesseract_scene_graph/graph.h>
#include <tesseract_scene_graph/kinematics_information.h>

namespace tesseract_scene_graph
{
/**
 * @brief Parse group Robot with External Positioner kinematics from srdf xml element
 * @param scene_graph The tesseract scene graph
 * @param srdf_xml The xml element to parse
 * @param version The srdf version number
 * @return Group OPW kinematics parameters
 */
inline GroupREPKinematics parseGroupREPKinematics(const tesseract_scene_graph::SceneGraph& /*scene_graph*/,
                                                  const tinyxml2::XMLElement* srdf_xml,
                                                  const std::array<int, 3>& /*version*/)
{
  GroupREPKinematics group_rep_kinematics;

  for (const tinyxml2::XMLElement* xml_element = srdf_xml->FirstChildElement("group_rep"); xml_element;
       xml_element = xml_element->NextSiblingElement("group_rep"))
  {
    std::string group_name_string;
    tinyxml2::XMLError status = tesseract_common::QueryStringAttributeRequired(xml_element, "group", group_name_string);
    if (status != tinyxml2::XML_SUCCESS)
      continue;

    // get the robot with external positioner group information
    REPKinematicParameters rep_info;

    status = tesseract_common::QueryStringAttribute(xml_element, "solver_name", rep_info.solver_name);
    if (status != tinyxml2::XML_NO_ATTRIBUTE && status != tinyxml2::XML_SUCCESS)
    {
      CONSOLE_BRIDGE_logInform("REP Group, group_rep element failed to parse 'solver_name' attribute!");
      continue;
    }

    const tinyxml2::XMLElement* manip_xml = xml_element->FirstChildElement("manipulator");
    if (manip_xml == nullptr)
    {
      CONSOLE_BRIDGE_logError("REP Group defined, but missing manipulator element!");
      continue;
    }

    status = tesseract_common::QueryStringAttributeRequired(manip_xml, "group", rep_info.manipulator_group);
    if (status != tinyxml2::XML_SUCCESS)
      continue;

    status = tesseract_common::QueryStringAttributeRequired(manip_xml, "ik_solver", rep_info.manipulator_ik_solver);
    if (status != tinyxml2::XML_SUCCESS)
      continue;

    status = tesseract_common::QueryDoubleAttributeRequired(manip_xml, "reach", rep_info.manipulator_reach);
    if (status != tinyxml2::XML_SUCCESS)
      continue;

    const tinyxml2::XMLElement* positioner_xml = xml_element->FirstChildElement("positioner");
    if (positioner_xml == nullptr)
    {
      CONSOLE_BRIDGE_logError("REP Group defined, but missing positioner element!");
      continue;
    }

    status = tesseract_common::QueryStringAttributeRequired(positioner_xml, "group", rep_info.positioner_group);
    if (status != tinyxml2::XML_SUCCESS)
      continue;

    status = tesseract_common::QueryStringAttribute(positioner_xml, "fk_solver", rep_info.positioner_fk_solver);
    if (status != tinyxml2::XML_NO_ATTRIBUTE && status != tinyxml2::XML_SUCCESS)
    {
      CONSOLE_BRIDGE_logInform("REP Group, positioner element missing or failed to parse 'fk_solver' attribute!");
      continue;
    }

    // get the chains in the groups
    bool parse_joints_failed = false;
    for (const tinyxml2::XMLElement* joint_xml = positioner_xml->FirstChildElement("joint"); positioner_xml;
         positioner_xml = positioner_xml->NextSiblingElement("joint"))
    {
      if (joint_xml == nullptr)
      {
        parse_joints_failed = true;
        continue;
      }

      std::string joint_name;
      status = tesseract_common::QueryStringAttributeRequired(joint_xml, "name", joint_name);
      if (status != tinyxml2::XML_SUCCESS)
      {
        parse_joints_failed = true;
        continue;
      }

      double resolution;
      status = tesseract_common::QueryDoubleAttributeRequired(joint_xml, "resolution", resolution);
      if (status != tinyxml2::XML_SUCCESS)
      {
        parse_joints_failed = true;
        continue;
      }

      rep_info.positioner_sample_resolution[joint_name] = resolution;
    }

    if (parse_joints_failed || rep_info.positioner_sample_resolution.empty())
      continue;

    group_rep_kinematics[group_name_string] = rep_info;
  }

  return group_rep_kinematics;
}
}  // namespace tesseract_scene_graph
#endif  // TESSERACT_SCENE_GRAPH_SRDF_GROUP_REP_KINEMATICS_H
