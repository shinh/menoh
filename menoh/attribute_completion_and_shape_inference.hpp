
#ifndef MENOH_ATTRIBUTE_COMPLETION_AND_SHAPE_INFERENCE_HPP
#define MENOH_ATTRIBUTE_COMPLETION_AND_SHAPE_INFERENCE_HPP
/*
 * This file is generated by gen_attribute_completion_and_shape_inference_hpp.py
 * Do NOT modify this file directly
 */
#include <algorithm>
#include <cassert>
#include <numeric> // for accumulate
#include <string>
#include <unordered_map>

#include <menoh/array.hpp>
#include <menoh/model_data.hpp>
#include <menoh/utility.hpp>

namespace menoh_impl {
    inline auto complete_attribute_and_infer_shape(
            model_data& model_data,
            std::unordered_map<std::string, array_profile> const&
                input_profile_table) {
        using ints = std::vector<int>;
        std::unordered_map<std::string, array_profile> variable_profile_table(
            input_profile_table.begin(), input_profile_table.end());
        std::transform(
            model_data.parameter_name_and_array_list.begin(),
            model_data.parameter_name_and_array_list.end(),
            std::inserter(variable_profile_table,
                          variable_profile_table.end()),
            [](auto const& p){
                return std::make_pair(
                    p.first,
                    array_profile(p.second.dtype(), p.second.dims())); });
        auto profile_of = [&variable_profile_table](std::string const& name){
            if(variable_profile_table.find(name) ==
                variable_profile_table.end()) {
                throw variable_not_found(name);
            }
            return variable_profile_table.at(name);
        };
        auto dims_of = [&variable_profile_table, profile_of](
            std::string const& name){
                return profile_of(name).dims();
        };
        auto dtype_of = [&variable_profile_table, profile_of](
            std::string const& name){
                return profile_of(name).dtype();
        };
        auto ndims_of = [&dims_of](std::string const& parameter_name) {
            return dims_of(parameter_name).size();
        };
        auto add_variable_to_table = [&variable_profile_table](
            std::string const& name,
            dtype_t dtype, ints const& dims){
                variable_profile_table.emplace(
                    name, array_profile(dtype, dims));
            };

        auto graph = make_graph(model_data.node_list); // FIXME reorder nodes
        model_data.node_list = graph.node_list();
        for(auto& node : model_data.node_list) {
            auto input = [&node](auto i){
                return node.input_name_list.at(i);
            };
            auto output = [&node](auto i){
                return node.output_name_list.at(i);
            };
            
if(node.op_type == "Abs") {
    
    
    
    {
        
        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "Add") {
    
    
    
    {
        
        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "AveragePool") {
    
assert(2 <= ndims_of(input(0)));

    
{
    auto found = node.attribute_table.find("count_include_pad");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "count_include_pad", 0);

    }
}


{
    auto found = node.attribute_table.find("kernel_shape");
    if(found == node.attribute_table.end()) {
        
assert(!"attribute not found: kernel_shape");

    }
}


{
    auto found = node.attribute_table.find("pads");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "pads", ints(2*(ndims_of(input(0))-2), 0));

    }
}


{
    auto found = node.attribute_table.find("strides");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "strides", ints(ndims_of(input(0))-2, 1));

    }
}

    
    {
        
auto count_include_pad = get<int>(node.attribute_table.at("count_include_pad"));
static_cast<void>(count_include_pad); // maybe unused


auto kernel_shape = get<ints>(node.attribute_table.at("kernel_shape"));
static_cast<void>(kernel_shape); // maybe unused


auto pads = get<ints>(node.attribute_table.at("pads"));
static_cast<void>(pads); // maybe unused


auto strides = get<ints>(node.attribute_table.at("strides"));
static_cast<void>(strides); // maybe unused

        
add_variable_to_table(output(0), dtype_of(input(0)),
    calc_2d_output_dims(
        dims_of(input(0)), dims_of(input(0)).at(1),
        kernel_shape, strides, pads));

    }
}
else


if(node.op_type == "BatchNormalization") {
    
    
{
    auto found = node.attribute_table.find("epsilon");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "epsilon", 1.e-05f);

    }
}


{
    auto found = node.attribute_table.find("momentum");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "momentum", 0.9f);

    }
}


{
    auto found = node.attribute_table.find("spatial");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "spatial", 1);

    }
}

    
    {
        
auto epsilon = get<float>(node.attribute_table.at("epsilon"));
static_cast<void>(epsilon); // maybe unused


auto momentum = get<float>(node.attribute_table.at("momentum"));
static_cast<void>(momentum); // maybe unused


auto spatial = get<int>(node.attribute_table.at("spatial"));
static_cast<void>(spatial); // maybe unused

        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "Concat") {
    
    
{
    auto found = node.attribute_table.find("axis");
    if(found == node.attribute_table.end()) {
        
assert(!"attribute not found: axis");

    }
}

    
    {
        
auto axis = get<int>(node.attribute_table.at("axis"));
static_cast<void>(axis); // maybe unused

        
auto output_dims = dims_of(input(0));
for(unsigned int i = 1; i < node.input_name_list.size(); ++i) {
    // TODO dim check
    output_dims.at(axis) += dims_of(input(i)).at(axis);
}
add_variable_to_table(output(0), dtype_of(input(0)), output_dims);

    }
}
else


if(node.op_type == "Conv") {
    
auto kernel_ndims = ndims_of(input(1))-2;
auto weights_shape = dims_of(input(1));
auto kernel_shape = ints(weights_shape.begin()+2, weights_shape.end());

    
{
    auto found = node.attribute_table.find("dilations");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "dilations", ints(kernel_ndims, 1));

    }
}


{
    auto found = node.attribute_table.find("group");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "group", 1);

    }
}


{
    auto found = node.attribute_table.find("kernel_shape");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "kernel_shape", kernel_shape);

    }
}


{
    auto found = node.attribute_table.find("pads");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "pads", ints(kernel_ndims*2, 0));

    }
}


{
    auto found = node.attribute_table.find("strides");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "strides", ints(kernel_ndims, 1));

    }
}

    
    {
        
auto dilations = get<ints>(node.attribute_table.at("dilations"));
static_cast<void>(dilations); // maybe unused


auto group = get<int>(node.attribute_table.at("group"));
static_cast<void>(group); // maybe unused


auto kernel_shape = get<ints>(node.attribute_table.at("kernel_shape"));
static_cast<void>(kernel_shape); // maybe unused


auto pads = get<ints>(node.attribute_table.at("pads"));
static_cast<void>(pads); // maybe unused


auto strides = get<ints>(node.attribute_table.at("strides"));
static_cast<void>(strides); // maybe unused

        
add_variable_to_table(output(0), dtype_of(input(0)),
    calc_2d_output_dims(
        dims_of(input(0)), dims_of(input(1)).at(0),
        kernel_shape, strides, pads));

    }
}
else


if(node.op_type == "ConvTranspose") {
    
auto kernel_ndims = ndims_of(input(1))-2;
auto weights_shape = dims_of(input(1));
auto kernel_shape = ints(weights_shape.begin()+2, weights_shape.end());

    
{
    auto found = node.attribute_table.find("dilations");
    if(found == node.attribute_table.end()) {
        
assert(!"attribute not found: dilations");

    }
}


{
    auto found = node.attribute_table.find("group");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "group", 1);

    }
}


{
    auto found = node.attribute_table.find("kernel_shape");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "kernel_shape", kernel_shape);

    }
}


{
    auto found = node.attribute_table.find("output_padding");
    if(found == node.attribute_table.end()) {
        
assert(!"attribute not found: output_padding");

    }
}


{
    auto found = node.attribute_table.find("strides");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "strides", ints(kernel_ndims, 1));

    }
}

    
{
    auto found = node.attribute_table.find("output_shape");
    assert(!(found == node.attribute_table.end() &&
       node.attribute_table.find("pads") == node.attribute_table.end()));
    if(found != node.attribute_table.end()) {
        auto output_shape = get<ints>(found->second);
        /* [dim0_begin, dim1_begin, ... , dim0_end, dim1_end, ..., ...] */
        ints pads(kernel_ndims*2, 0);
        auto output_padding =
            get<ints>(node.attribute_table.at("output_padding"));
        auto strides = get<ints>(node.attribute_table.at("strides"));
        auto input_profile = input_profile_table.at(input(0));
        ints input_size(input_profile.dims().begin()+2,
                        input_profile.dims().end());

        for(unsigned int i = 0; i < kernel_ndims; ++i) {
            auto total_padding = strides[i] * (input_size[i] - 1)
                + output_padding[i] + kernel_shape[i] - output_shape[i];
            pads[i] = total_padding - (total_padding/2);
            pads[i+kernel_ndims] = (total_padding/2);
        }

        node.attribute_table["pads"] = pads;
    }
}

    {
        
auto dilations = get<ints>(node.attribute_table.at("dilations"));
static_cast<void>(dilations); // maybe unused


auto group = get<int>(node.attribute_table.at("group"));
static_cast<void>(group); // maybe unused


auto kernel_shape = get<ints>(node.attribute_table.at("kernel_shape"));
static_cast<void>(kernel_shape); // maybe unused


auto output_padding = get<ints>(node.attribute_table.at("output_padding"));
static_cast<void>(output_padding); // maybe unused


auto strides = get<ints>(node.attribute_table.at("strides"));
static_cast<void>(strides); // maybe unused

        
add_variable_to_table(output(0), dtype_of(input(0)),
    calc_2d_output_dims_for_conv_transpose(
        dims_of(input(0)), dims_of(input(1)).at(0),
        kernel_shape, strides, get<ints>(node.attribute_table.at("pads"))));

    }
}
else


if(node.op_type == "Elu") {
    
    
{
    auto found = node.attribute_table.find("alpha");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "alpha", 1.f);

    }
}

    
    {
        
auto alpha = get<float>(node.attribute_table.at("alpha"));
static_cast<void>(alpha); // maybe unused

        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "FC") {
    
    
    
    {
        
        
auto output_dims = ints({dims_of(input(0)).at(0), dims_of(input(1)).at(0)});
add_variable_to_table(output(0), dtype_of(input(0)),
    output_dims);

    }
}
else


if(node.op_type == "Gemm") {
    
    
{
    auto found = node.attribute_table.find("alpha");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "alpha", 1.f);

    }
}


{
    auto found = node.attribute_table.find("beta");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "beta", 1.f);

    }
}


{
    auto found = node.attribute_table.find("transA");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "transA", 0);

    }
}


{
    auto found = node.attribute_table.find("transB");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "transB", 0);

    }
}

    
    {
        
auto alpha = get<float>(node.attribute_table.at("alpha"));
static_cast<void>(alpha); // maybe unused


auto beta = get<float>(node.attribute_table.at("beta"));
static_cast<void>(beta); // maybe unused


auto transA = get<int>(node.attribute_table.at("transA"));
static_cast<void>(transA); // maybe unused


auto transB = get<int>(node.attribute_table.at("transB"));
static_cast<void>(transB); // maybe unused

        
auto a_dims = dims_of(input(0));
if(ndims_of(input(0)) != 2) {
    int feature_size = std::accumulate(
        a_dims.begin()+1, a_dims.end(), 1, std::multiplies<int>());
    a_dims = ints({a_dims.at(0), feature_size});
}
assert(a_dims.size() == 2);
if(transA) {
    std::swap(a_dims.at(0), a_dims.at(1));
}

auto b_dims = dims_of(input(1));
assert(b_dims.size() == 2);
if(transB) {
    std::swap(b_dims.at(0), b_dims.at(1));
}

if(a_dims.at(1) != b_dims.at(0)) {
    throw dimension_mismatch(
        node.op_type, input(0), "trans(A)[1] and trans(B)[0])",
        std::to_string(a_dims.at(1)), std::to_string(b_dims.at(0)));
}

auto output_dims = ints({a_dims.at(0), b_dims.at(1)});
add_variable_to_table(output(0), dtype_of(input(0)), output_dims);

    }
}
else


if(node.op_type == "LeakyRelu") {
    
    
{
    auto found = node.attribute_table.find("alpha");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "alpha", 0.01f);

    }
}

    
    {
        
auto alpha = get<float>(node.attribute_table.at("alpha"));
static_cast<void>(alpha); // maybe unused

        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "LRN") {
    
    
{
    auto found = node.attribute_table.find("alpha");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "alpha", 0.0001f);

    }
}


{
    auto found = node.attribute_table.find("beta");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "beta", 0.75f);

    }
}


{
    auto found = node.attribute_table.find("bias");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "bias", 1.0f);

    }
}


{
    auto found = node.attribute_table.find("size");
    if(found == node.attribute_table.end()) {
        
assert(!"attribute not found: size");

    }
}

    
    {
        
auto alpha = get<float>(node.attribute_table.at("alpha"));
static_cast<void>(alpha); // maybe unused


auto beta = get<float>(node.attribute_table.at("beta"));
static_cast<void>(beta); // maybe unused


auto bias = get<float>(node.attribute_table.at("bias"));
static_cast<void>(bias); // maybe unused


auto size = get<float>(node.attribute_table.at("size"));
static_cast<void>(size); // maybe unused

        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "MaxPool") {
    
    
{
    auto found = node.attribute_table.find("kernel_shape");
    if(found == node.attribute_table.end()) {
        
assert(!"attribute not found: kernel_shape");

    }
}


{
    auto found = node.attribute_table.find("pads");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "pads", ints(2*(ndims_of(input(0))-2), 0));

    }
}


{
    auto found = node.attribute_table.find("storage_order");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "storage_order", 0);

    }
}


{
    auto found = node.attribute_table.find("strides");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "strides", ints(ndims_of(input(0))-2, 1));

    }
}

    
    {
        
auto kernel_shape = get<ints>(node.attribute_table.at("kernel_shape"));
static_cast<void>(kernel_shape); // maybe unused


auto pads = get<ints>(node.attribute_table.at("pads"));
static_cast<void>(pads); // maybe unused


auto storage_order = get<int>(node.attribute_table.at("storage_order"));
static_cast<void>(storage_order); // maybe unused


auto strides = get<ints>(node.attribute_table.at("strides"));
static_cast<void>(strides); // maybe unused

        
add_variable_to_table(output(0), dtype_of(input(0)),
    calc_2d_output_dims(
        dims_of(input(0)), dims_of(input(0)).at(1),
        kernel_shape, strides, pads));

    }
}
else


if(node.op_type == "Relu") {
    
    
    
    {
        
        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "Sigmoid") {
    
    
    
    {
        
        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "Softmax") {
    
    
{
    auto found = node.attribute_table.find("axis");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "axis", 1);

    }
}

    
    {
        
auto axis = get<int>(node.attribute_table.at("axis"));
static_cast<void>(axis); // maybe unused

        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "Sum") {
    
    
    
    {
        
        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "Sqrt") {
    
    
    
    {
        
        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "Tanh") {
    
    
    
    {
        
        
assert(node.input_name_list.size() > 0);
assert(node.output_name_list.size() > 0);
add_variable_to_table(output(0), dtype_of(input(0)), dims_of(input(0)));

    }
}
else


if(node.op_type == "Transpose") {
    
ints perm(ndims_of(input(0)));
for(unsigned int i = 0; i < perm.size(); ++i) {{
    perm.at(i) = perm.size()-i-1;
}}

    
{
    auto found = node.attribute_table.find("perm");
    if(found == node.attribute_table.end()) {
        
node.attribute_table.emplace(
    "perm", perm);

    }
}

    
    {
        
auto perm = get<ints>(node.attribute_table.at("perm"));
static_cast<void>(perm); // maybe unused

        
auto input_dims = dims_of(input(0));
ints output_dims(input_dims.size());
for(unsigned int i = 0; i < input_dims.size(); ++i) {
    output_dims.at(i) = input_dims.at(perm.at(i));
}
add_variable_to_table(output(0), dtype_of(input(0)), output_dims);

    }
}
else

            
{
    throw unsupported_operator(node.op_type);
}

        }
        return variable_profile_table;
    }
} // namespace menoh_impl

#endif // MENOH_ATTRIBUTE_COMPLETION_AND_SHAPE_INFERENCE_HPP

