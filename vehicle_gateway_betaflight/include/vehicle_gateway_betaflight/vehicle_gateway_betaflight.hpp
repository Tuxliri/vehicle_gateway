// Copyright 2023 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef VEHICLE_GATEWAY_BETAFLIGHT__VEHICLE_GATEWAY_BETAFLIGHT_HPP_
#define VEHICLE_GATEWAY_BETAFLIGHT__VEHICLE_GATEWAY_BETAFLIGHT_HPP_

#include "vehicle_gateway/vehicle_gateway.hpp"

#include <limits>
#include <memory>
#include <vector>

#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/magnetic_field.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/u_int16_multi_array.hpp>

#include <rclcpp/rclcpp.hpp>

#include <msp/FlightController.hpp>
#include <msp/msp_msg.hpp>

namespace vehicle_gateway_betaflight
{

class VehicleGatewayBetaflight : public vehicle_gateway::VehicleGateway
{
public:
  /// Documentation inherited
  void init(int argc, const char ** argv) override;

  /// Documentation inherited
  void set_vehicle_id(unsigned int _vehicle_id) override;

  /// Documentation inherited
  unsigned int get_vehicle_id() override;

  /// Documentation inherited
  void destroy() override;

  /// Documentation inherited
  void arm() override;

  /// Documentation inherited
  void arm_sync() override;

  /// Documentation inherited
  void disarm() override;

  /// Documentation inherited
  void disarm_sync() override;

  /// Documentation inherited
  vehicle_gateway::ARMING_STATE get_arming_state() override;

  /// Documentation inherited
  vehicle_gateway::FLIGHT_MODE get_flight_mode() override;

  /// Documentation inherited
  vehicle_gateway::VEHICLE_TYPE get_vehicle_type() override;

  /// Documentation inherited
  vehicle_gateway::ARM_DISARM_REASON get_arm_reason() override;

  /// Documentation inherited
  vehicle_gateway::ARM_DISARM_REASON get_disarm_reason() override;

  /// Documentation inherited
  vehicle_gateway::FAILURE get_failure() override;

  /// Documentation inherited
  vehicle_gateway::VTOL_STATE get_vtol_state() override;

  /// Documentation inherited
  void takeoff() override;

  /// Documentation inherited
  void land() override;

  /// Documentation inherited
  void transition_to_fw() override;

  /// Documentation inherited
  void transition_to_fw_sync() override;

  /// Documentation inherited
  void transition_to_mc() override;

  /// Documentation inherited
  void transition_to_mc_sync() override;

  /// Document inherited
  void go_to_latlon(double lat, double lon, float alt_amsl) override;

  /// Documentation inherited
  void go_to_latlon_sync(
    double lat, double lon, double alt,
    double latlon_threshold = 0.5, double alt_threshold = 0.5) override;

  /// Documentation inherited
  void set_local_position_setpoint(float x, float y, float z, float yaw) override;

  void offboard_mode_go_to_local_setpoint_sync(
    double x,
    double y,
    double alt,
    double yaw = std::numeric_limits<float>::quiet_NaN(),
    double airspeeed = 15.0,
    double distance_threshold = 10.0,
    vehicle_gateway::CONTROLLER_TYPE controller_type = vehicle_gateway::CONTROLLER_TYPE::POSITION)
  override;

  /// Documentation inherited
  void set_local_velocity_setpoint(float vx, float vy, float vz, float yaw_rate = 0.0f) override;

  /// Documentation inherited
  void set_offboard_control_mode(vehicle_gateway::CONTROLLER_TYPE type) override;

  /// Documentation inherited
  void set_offboard_mode() override;

  /// Documentation inherited
  void transition_to_offboard_sync() override;

  /// Documentation inherited
  void set_onboard_mode() override;

  /// Documentation inherited
  void set_ground_speed(float speed) override;

  /// Documentation inherited
  void set_airspeed(float speed) override;

  /// Documentation inherited
  float get_altitude() override;

  /// Documentation inherited
  float get_ground_speed() override;

  /// Documentation inherited
  float get_airspeed() override;

  /// Documentation inherited
  void get_local_position(float & x, float & y, float & z) override;

  /// Documentation inherited
  void get_euler_rpy(float & roll, float & pitch, float & yaw) override;

  /// Documentation inherited
  std::vector<double> get_latlon() override;

  /// Documentation inherited
  bool set_body_rates_and_thrust_setpoint(
    float roll_rate, float pitch_rate, float yaw_rate, float thrust) override;

  /// Documentation inherited
  bool set_motors(std::vector<uint16_t> motor_values) override;

  bool create_multirobot_session(const char * config_filename) override {return false;}

  bool destroy_multirobot_session() override {return false;}

  void * get_multirobot_session() override {return nullptr;}

private:
  // Orchestration
  std::thread spin_thread_;
  std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> exec_;
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr pub_imu_raw_;
  rclcpp::Publisher<sensor_msgs::msg::MagneticField>::SharedPtr pub_imu_mag_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_altitude_;
  rclcpp::Publisher<std_msgs::msg::UInt16MultiArray>::SharedPtr pub_motors_;
  rclcpp::Node::SharedPtr betaflight_node_;

  // Service clients
  vehicle_gateway::ARMING_STATE arming_state_{vehicle_gateway::ARMING_STATE::MAX};

  void onStatus(const msp::msg::Status & status);
  void onBoxNames(const msp::msg::BoxNames & box_names);
  void onImu(const msp::msg::RawImu & imu);
  void onAltitude(const msp::msg::Altitude & altitude);
  void onMotor(const msp::msg::Motor & motor);

  fcu::FlightController fcu_;

  float roll_{0};
  float pitch_{0};
  float yaw_{0};
  float throttle_{0};
  uint16_t arm_{0};
  float altitude{0};

  unsigned int vehicle_id_{0};

  int index_box_arm_{-1};
};
}  // namespace vehicle_gateway_betaflight

#endif  // VEHICLE_GATEWAY_BETAFLIGHT__VEHICLE_GATEWAY_BETAFLIGHT_HPP_
