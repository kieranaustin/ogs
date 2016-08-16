/**
 * \copyright
 * Copyright (c) 2012-2016, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#include "MeshElementParameter.h"
#include "BaseLib/ConfigTree.h"
#include "BaseLib/Error.h"
#include "MeshLib/Mesh.h"

namespace ProcessLib
{
std::unique_ptr<ParameterBase> createMeshElementParameter(
    BaseLib::ConfigTree const& config, MeshLib::Mesh const& mesh)
{
    //! \ogs_file_param{parameter__type}
    config.checkConfigParameter("type", "MeshProperty");
    //! \ogs_file_param{parameter__MeshProperty__field_name}
    auto field_name = config.getConfigParameter<std::string>("field_name");
    DBUG("Using field_name %s", field_name.c_str());

    if (!mesh.getProperties().hasPropertyVector(field_name)) {
        OGS_FATAL("The required property %s does not exists in the mesh.",
                  field_name.c_str());
    }

    // TODO other data types than only double
    auto const& property =
        mesh.getProperties().template getPropertyVector<double>(field_name);
    if (!property) {
        OGS_FATAL("The required property %s is not of the requested type.",
                  field_name.c_str());
    }

    return std::unique_ptr<ParameterBase>(
        new MeshElementParameter<double>(*property));
}

}  // ProcessLib
