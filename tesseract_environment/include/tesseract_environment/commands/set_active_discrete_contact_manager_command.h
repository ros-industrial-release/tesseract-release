/**
 * @file set_active_discrete_contact_manager_command.h
 * @brief Used to set the active discrete contact manager for the environment
 *
 * @author Levi Armstrong
 * @date October 26, 2021
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2021, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_ENVIRONMENT_SET_ACTIVE_DISCRETE_CONTACT_MANAGER_COMMAND_H
#define TESSERACT_ENVIRONMENT_SET_ACTIVE_DISCRETE_CONTACT_MANAGER_COMMAND_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <boost/serialization/access.hpp>
#include <memory>
#include <string>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_environment/command.h>

namespace tesseract_environment
{
class SetActiveDiscreteContactManagerCommand : public Command
{
public:
  using Ptr = std::shared_ptr<SetActiveDiscreteContactManagerCommand>;
  using ConstPtr = std::shared_ptr<const SetActiveDiscreteContactManagerCommand>;

  SetActiveDiscreteContactManagerCommand() : Command(CommandType::SET_ACTIVE_DISCRETE_CONTACT_MANAGER){};

  /**
   * @brief Set the active discrete contact manager
   * @param active_contact_manager Active contact manager name
   */
  SetActiveDiscreteContactManagerCommand(std::string active_contact_manager)
    : Command(CommandType::SET_ACTIVE_DISCRETE_CONTACT_MANAGER)
    , active_contact_manager_(std::move(active_contact_manager))
  {
  }

  const std::string& getName() const { return active_contact_manager_; }

  bool operator==(const SetActiveDiscreteContactManagerCommand& rhs) const;
  bool operator!=(const SetActiveDiscreteContactManagerCommand& rhs) const;

private:
  std::string active_contact_manager_;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};
}  // namespace tesseract_environment

#include <boost/serialization/export.hpp>
#include <boost/serialization/tracking.hpp>
BOOST_CLASS_EXPORT_KEY2(tesseract_environment::SetActiveDiscreteContactManagerCommand,
                        "SetActiveDiscreteContactManagerCommand")
#endif  // TESSERACT_ENVIRONMENT_SET_ACTIVE_DISCRETE_CONTACT_MANAGER_COMMAND_H
