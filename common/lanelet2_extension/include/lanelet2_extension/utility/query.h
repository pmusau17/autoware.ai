/*
 * Copyright 2015-2019 Autoware Foundation. All rights reserved.
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
 * Authors: Simon Thompson, Ryohsuke Mitsudome
 */

#ifndef LANELET2_EXTENSION_UTILITY_QUERY_H
#define LANELET2_EXTENSION_UTILITY_QUERY_H

#include <lanelet2_core/LaneletMap.h>
#include <lanelet2_core/primitives/Lanelet.h>
#include <lanelet2_core/primitives/Area.h>
#include <lanelet2_core/primitives/LineString.h>
#include <lanelet2_core/primitives/Point.h>
#include <lanelet2_core/primitives/RegulatoryElement.h>
#include <lanelet2_core/primitives/BasicRegulatoryElements.h>
#include <lanelet2_core/primitives/Primitive.h>

#include <geometry_msgs/PolygonStamped.h>

#include <lanelet2_extension/regulatory_elements/autoware_traffic_light.h>

#include <vector>
#include <string>
#include <unordered_set>

namespace lanelet
{
using TrafficLightConstPtr = std::shared_ptr<const lanelet::TrafficLight>;
using AutowareTrafficLightConstPtr = std::shared_ptr<const lanelet::autoware::AutowareTrafficLight>;
}  // namespace lanelet

namespace lanelet
{
namespace utils
{
namespace query
{
enum direction {CHECK_CHILD,CHECK_PARENT};
struct References
{
    References() {};
    struct comparator
    {
        template <typename PrimT>
        bool operator()(const PrimT& prim1, const PrimT& prim2) const {
            return prim1.id() == prim2.id();
        }
        bool operator()(const lanelet::RegulatoryElementConstPtr& prim1, 
        const lanelet::RegulatoryElementConstPtr& prim2) const {
            return prim1->id() == prim2->id();
        }
    };
    std::unordered_set<lanelet::ConstLineString3d, std::hash<lanelet::ConstLineString3d>, comparator> lss;
    std::unordered_set<lanelet::ConstLanelet, std::hash<lanelet::ConstLanelet>, comparator> llts;
    std::unordered_set<lanelet::ConstArea, std::hash<lanelet::ConstArea>, comparator> areas;
    std::unordered_set<lanelet::RegulatoryElementConstPtr, std::hash<lanelet::RegulatoryElementConstPtr>, comparator> regems;
};
/**
 * [findReferences finds all primitives that reference the given primitive in a given map]
 * @param  ll_Map [input lanelet map]
 * @return        [References object with referenced element sets for each primitive layers]
 * NOTE: Polygons and Compound primitives such as LaneletOrArea are not currently supported
 */
template <class primT>
References findReferences (const primT& prim, const lanelet::LaneletMapPtr ll_Map);

/**
 * [laneletLayer converts laneletLayer into lanelet vector]
 * @param  ll_Map [input lanelet map]
 * @return        [all lanelets in the map]
 */
lanelet::ConstLanelets laneletLayer(const lanelet::LaneletMapPtr ll_Map);

/**
 * [subtypeLanelets extracts Lanelet that has given subtype attribute]
 * @param  lls     [input lanelets with various subtypes]
 * @param  subtype [subtype of lanelets to be retrieved (e.g.
 * lanelet::AttributeValueString::Road)]
 * @return         [lanelets with given subtype]
 */
lanelet::ConstLanelets subtypeLanelets(const lanelet::ConstLanelets lls, const char subtype[]);

/**
 * [crosswalkLanelets extracts crosswalk lanelets]
 * @param  lls [input lanelets with various subtypes]
 * @return     [crosswalk lanelets]
 */
lanelet::ConstLanelets crosswalkLanelets(const lanelet::ConstLanelets lls);

/**
 * [roadLanelets extracts road lanelets]
 * @param  lls [input lanelets with subtype road]
 * @return     [road lanelets]
 */
lanelet::ConstLanelets roadLanelets(const lanelet::ConstLanelets lls);

/**
 * [trafficLights extracts Traffic Light regulatory element from lanelets]
 * @param lanelets [input lanelets]
 * @return         [traffic light that are associated with input lanenets]
 */
std::vector<lanelet::TrafficLightConstPtr> trafficLights(const lanelet::ConstLanelets lanelets);

/**
 * [autowareTrafficLights extracts Autoware Traffic Light regulatory element
 * from lanelets]
 * @param lanelets [input lanelets]
 * @return         [autoware traffic light that are associated with input
 * lanenets]
 */
std::vector<lanelet::AutowareTrafficLightConstPtr> autowareTrafficLights(const lanelet::ConstLanelets lanelets);

/**
 * [stopLinesLanelets extracts stoplines that are associated to lanelets]
 * @param lanelets [input lanelets]
 * @return         [stop lines that are associated with input lanelets]
 */
std::vector<lanelet::ConstLineString3d> stopLinesLanelets(const lanelet::ConstLanelets lanelets);

/**
 * [stopLinesLanelet extracts stop lines that are associated with a given
 * lanelet]
 * @param ll [input lanelet]
 * @return   [stop lines that are associated with input lanelet]
 */
std::vector<lanelet::ConstLineString3d> stopLinesLanelet(const lanelet::ConstLanelet ll);

/**
 * [stopSignes extracts stoplines that are associated with stopsignes]
 * @param lanelets     [input lanelets]
 * @param stop_sign_id [sign id of stop sign]
 * @return             [array of stoplines]
 */
std::vector<lanelet::ConstLineString3d> stopSignStopLines(const lanelet::ConstLanelets lanelets,
                                                          const std::string& stop_sign_id = "stop_sign");

}  // namespace query
}  // namespace utils
}  // namespace lanelet

// Template functions cannot be linked unless the implementation is provided
// Therefore include implementation to allow for template functions
#include "internal/query.tpp"

#endif  // LANELET2_EXTENSION_UTILITY_QUERY_H
