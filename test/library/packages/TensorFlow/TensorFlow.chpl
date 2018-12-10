module TensorFlow {
  module C_TensorFlow {
    require  "tensorflow/c/c_api.h", "-ltensorflow";

    // Note: Generated with fake std headers

    extern proc TF_Version() : c_string;

    extern proc TF_DataTypeSize(dt : TF_DataType) : size_t;

    extern proc TF_NewStatus() : c_ptr(TF_Status);

    extern proc TF_DeleteStatus(ref arg0 : TF_Status) : void;

    extern proc TF_DeleteStatus(arg0 : c_ptr(TF_Status)) : void;

    extern proc TF_SetStatus(ref s : TF_Status, code : TF_Code, msg : c_string) : void;

    extern proc TF_SetStatus(s : c_ptr(TF_Status), code : TF_Code, msg : c_string) : void;

    extern proc TF_GetCode(ref s : TF_Status) : TF_Code;

    extern proc TF_GetCode(s : c_ptr(TF_Status)) : TF_Code;

    extern proc TF_Message(ref s : TF_Status) : c_string;

    extern proc TF_Message(s : c_ptr(TF_Status)) : c_string;

    extern proc TF_NewBufferFromString(proto : c_void_ptr, proto_len : size_t) : c_ptr(TF_Buffer);

    extern proc TF_NewBuffer() : c_ptr(TF_Buffer);

    extern proc TF_DeleteBuffer(ref arg0 : TF_Buffer) : void;

    extern proc TF_DeleteBuffer(arg0 : c_ptr(TF_Buffer)) : void;

    extern proc TF_GetBuffer(ref buffer : TF_Buffer) : TF_Buffer;

    extern proc TF_GetBuffer(buffer : c_ptr(TF_Buffer)) : TF_Buffer;

    extern proc TF_NewTensor(arg0 : TF_DataType, ref dims : int(64), num_dims : c_int, data : c_void_ptr, len : size_t, ref deallocator : c_fn_ptr, deallocator_arg : c_void_ptr) : c_ptr(TF_Tensor);

    extern proc TF_NewTensor(arg0 : TF_DataType, dims : c_ptr(int(64)), num_dims : c_int, data : c_void_ptr, len : size_t, deallocator : c_fn_ptr, deallocator_arg : c_void_ptr) : c_ptr(TF_Tensor);

    extern proc TF_AllocateTensor(arg0 : TF_DataType, ref dims : int(64), num_dims : c_int, len : size_t) : c_ptr(TF_Tensor);

    extern proc TF_AllocateTensor(arg0 : TF_DataType, dims : c_ptr(int(64)), num_dims : c_int, len : size_t) : c_ptr(TF_Tensor);

    extern proc TF_TensorMaybeMove(ref tensor : TF_Tensor) : c_ptr(TF_Tensor);

    extern proc TF_TensorMaybeMove(tensor : c_ptr(TF_Tensor)) : c_ptr(TF_Tensor);

    extern proc TF_DeleteTensor(ref arg0 : TF_Tensor) : void;

    extern proc TF_DeleteTensor(arg0 : c_ptr(TF_Tensor)) : void;

    extern proc TF_TensorType(ref arg0 : TF_Tensor) : TF_DataType;

    extern proc TF_TensorType(arg0 : c_ptr(TF_Tensor)) : TF_DataType;

    extern proc TF_NumDims(ref arg0 : TF_Tensor) : c_int;

    extern proc TF_NumDims(arg0 : c_ptr(TF_Tensor)) : c_int;

    extern proc TF_Dim(ref tensor : TF_Tensor, dim_index : c_int) : int(64);

    extern proc TF_Dim(tensor : c_ptr(TF_Tensor), dim_index : c_int) : int(64);

    extern proc TF_TensorByteSize(ref arg0 : TF_Tensor) : size_t;

    extern proc TF_TensorByteSize(arg0 : c_ptr(TF_Tensor)) : size_t;

    extern proc TF_TensorData(ref arg0 : TF_Tensor) : c_void_ptr;

    extern proc TF_TensorData(arg0 : c_ptr(TF_Tensor)) : c_void_ptr;

    extern proc TF_StringEncode(src : c_string, src_len : size_t, dst : c_string, dst_len : size_t, ref status : TF_Status) : size_t;

    extern proc TF_StringEncode(src : c_string, src_len : size_t, dst : c_string, dst_len : size_t, status : c_ptr(TF_Status)) : size_t;

    extern proc TF_StringDecode(src : c_string, src_len : size_t, ref dst : c_string, ref dst_len : size_t, ref status : TF_Status) : size_t;

    extern proc TF_StringDecode(src : c_string, src_len : size_t, dst : c_ptr(c_string), dst_len : c_ptr(size_t), status : c_ptr(TF_Status)) : size_t;

    extern proc TF_StringEncodedSize(len : size_t) : size_t;

    extern proc TF_NewSessionOptions() : c_ptr(TF_SessionOptions);

    extern proc TF_SetTarget(ref options : TF_SessionOptions, target : c_string) : void;

    extern proc TF_SetTarget(options : c_ptr(TF_SessionOptions), target : c_string) : void;

    extern proc TF_SetConfig(ref options : TF_SessionOptions, proto : c_void_ptr, proto_len : size_t, ref status : TF_Status) : void;

    extern proc TF_SetConfig(options : c_ptr(TF_SessionOptions), proto : c_void_ptr, proto_len : size_t, status : c_ptr(TF_Status)) : void;

    extern proc TF_DeleteSessionOptions(ref arg0 : TF_SessionOptions) : void;

    extern proc TF_DeleteSessionOptions(arg0 : c_ptr(TF_SessionOptions)) : void;

    extern proc TF_NewGraph() : c_ptr(TF_Graph);

    extern proc TF_DeleteGraph(ref arg0 : TF_Graph) : void;

    extern proc TF_DeleteGraph(arg0 : c_ptr(TF_Graph)) : void;

    extern proc TF_GraphSetTensorShape(ref graph : TF_Graph, output : TF_Output, ref dims : int(64), num_dims : c_int, ref status : TF_Status) : void;

    extern proc TF_GraphSetTensorShape(graph : c_ptr(TF_Graph), output : TF_Output, dims : c_ptr(int(64)), num_dims : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_GraphGetTensorNumDims(ref graph : TF_Graph, output : TF_Output, ref status : TF_Status) : c_int;

    extern proc TF_GraphGetTensorNumDims(graph : c_ptr(TF_Graph), output : TF_Output, status : c_ptr(TF_Status)) : c_int;

    extern proc TF_GraphGetTensorShape(ref graph : TF_Graph, output : TF_Output, ref dims : int(64), num_dims : c_int, ref status : TF_Status) : void;

    extern proc TF_GraphGetTensorShape(graph : c_ptr(TF_Graph), output : TF_Output, dims : c_ptr(int(64)), num_dims : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_NewOperation(ref graph : TF_Graph, op_type : c_string, oper_name : c_string) : c_ptr(TF_OperationDescription);

    extern proc TF_NewOperation(graph : c_ptr(TF_Graph), op_type : c_string, oper_name : c_string) : c_ptr(TF_OperationDescription);

    extern proc TF_SetDevice(ref desc : TF_OperationDescription, device : c_string) : void;

    extern proc TF_SetDevice(desc : c_ptr(TF_OperationDescription), device : c_string) : void;

    extern proc TF_AddInput(ref desc : TF_OperationDescription, input : TF_Output) : void;

    extern proc TF_AddInput(desc : c_ptr(TF_OperationDescription), input : TF_Output) : void;

    extern proc TF_AddInputList(ref desc : TF_OperationDescription, ref inputs : TF_Output, num_inputs : c_int) : void;

    extern proc TF_AddInputList(desc : c_ptr(TF_OperationDescription), inputs : c_ptr(TF_Output), num_inputs : c_int) : void;

    extern proc TF_AddControlInput(ref desc : TF_OperationDescription, ref input : TF_Operation) : void;

    extern proc TF_AddControlInput(desc : c_ptr(TF_OperationDescription), input : c_ptr(TF_Operation)) : void;

    extern proc TF_ColocateWith(ref desc : TF_OperationDescription, ref op : TF_Operation) : void;

    extern proc TF_ColocateWith(desc : c_ptr(TF_OperationDescription), op : c_ptr(TF_Operation)) : void;

    extern proc TF_SetAttrString(ref desc : TF_OperationDescription, attr_name : c_string, value : c_void_ptr, length : size_t) : void;

    extern proc TF_SetAttrString(desc : c_ptr(TF_OperationDescription), attr_name : c_string, value : c_void_ptr, length : size_t) : void;

    extern proc TF_SetAttrStringList(ref desc : TF_OperationDescription, attr_name : c_string, ref values : c_void_ptr, ref lengths : size_t, num_values : c_int) : void;

    extern proc TF_SetAttrStringList(desc : c_ptr(TF_OperationDescription), attr_name : c_string, values : c_ptr(c_void_ptr), lengths : c_ptr(size_t), num_values : c_int) : void;

    extern proc TF_SetAttrInt(ref desc : TF_OperationDescription, attr_name : c_string, value : int(64)) : void;

    extern proc TF_SetAttrInt(desc : c_ptr(TF_OperationDescription), attr_name : c_string, value : int(64)) : void;

    extern proc TF_SetAttrIntList(ref desc : TF_OperationDescription, attr_name : c_string, ref values : int(64), num_values : c_int) : void;

    extern proc TF_SetAttrIntList(desc : c_ptr(TF_OperationDescription), attr_name : c_string, values : c_ptr(int(64)), num_values : c_int) : void;

    extern proc TF_SetAttrFloat(ref desc : TF_OperationDescription, attr_name : c_string, value : c_float) : void;

    extern proc TF_SetAttrFloat(desc : c_ptr(TF_OperationDescription), attr_name : c_string, value : c_float) : void;

    extern proc TF_SetAttrFloatList(ref desc : TF_OperationDescription, attr_name : c_string, ref values : c_float, num_values : c_int) : void;

    extern proc TF_SetAttrFloatList(desc : c_ptr(TF_OperationDescription), attr_name : c_string, values : c_ptr(c_float), num_values : c_int) : void;

    extern proc TF_SetAttrBool(ref desc : TF_OperationDescription, attr_name : c_string, value : c_uchar) : void;

    extern proc TF_SetAttrBool(desc : c_ptr(TF_OperationDescription), attr_name : c_string, value : c_uchar) : void;

    extern proc TF_SetAttrBoolList(ref desc : TF_OperationDescription, attr_name : c_string, ref values : c_uchar, num_values : c_int) : void;

    extern proc TF_SetAttrBoolList(desc : c_ptr(TF_OperationDescription), attr_name : c_string, values : c_ptr(c_uchar), num_values : c_int) : void;

    extern proc TF_SetAttrType(ref desc : TF_OperationDescription, attr_name : c_string, value : TF_DataType) : void;

    extern proc TF_SetAttrType(desc : c_ptr(TF_OperationDescription), attr_name : c_string, value : TF_DataType) : void;

    extern proc TF_SetAttrTypeList(ref desc : TF_OperationDescription, attr_name : c_string, ref values : TF_DataType, num_values : c_int) : void;

    extern proc TF_SetAttrTypeList(desc : c_ptr(TF_OperationDescription), attr_name : c_string, values : c_ptr(TF_DataType), num_values : c_int) : void;

    extern proc TF_SetAttrFuncName(ref desc : TF_OperationDescription, attr_name : c_string, value : c_string, length : size_t) : void;

    extern proc TF_SetAttrFuncName(desc : c_ptr(TF_OperationDescription), attr_name : c_string, value : c_string, length : size_t) : void;

    extern proc TF_SetAttrShape(ref desc : TF_OperationDescription, attr_name : c_string, ref dims : int(64), num_dims : c_int) : void;

    extern proc TF_SetAttrShape(desc : c_ptr(TF_OperationDescription), attr_name : c_string, dims : c_ptr(int(64)), num_dims : c_int) : void;

    extern proc TF_SetAttrShapeList(ref desc : TF_OperationDescription, attr_name : c_string, ref dims : c_ptr(int(64)), ref num_dims : c_int, num_shapes : c_int) : void;

    extern proc TF_SetAttrShapeList(desc : c_ptr(TF_OperationDescription), attr_name : c_string, dims : c_ptr(c_ptr(int(64))), num_dims : c_ptr(c_int), num_shapes : c_int) : void;

    extern proc TF_SetAttrTensorShapeProto(ref desc : TF_OperationDescription, attr_name : c_string, proto : c_void_ptr, proto_len : size_t, ref status : TF_Status) : void;

    extern proc TF_SetAttrTensorShapeProto(desc : c_ptr(TF_OperationDescription), attr_name : c_string, proto : c_void_ptr, proto_len : size_t, status : c_ptr(TF_Status)) : void;

    extern proc TF_SetAttrTensorShapeProtoList(ref desc : TF_OperationDescription, attr_name : c_string, ref protos : c_void_ptr, ref proto_lens : size_t, num_shapes : c_int, ref status : TF_Status) : void;

    extern proc TF_SetAttrTensorShapeProtoList(desc : c_ptr(TF_OperationDescription), attr_name : c_string, protos : c_ptr(c_void_ptr), proto_lens : c_ptr(size_t), num_shapes : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_SetAttrTensor(ref desc : TF_OperationDescription, attr_name : c_string, ref value : TF_Tensor, ref status : TF_Status) : void;

    extern proc TF_SetAttrTensor(desc : c_ptr(TF_OperationDescription), attr_name : c_string, value : c_ptr(TF_Tensor), status : c_ptr(TF_Status)) : void;

    extern proc TF_SetAttrTensorList(ref desc : TF_OperationDescription, attr_name : c_string, ref values : c_ptr(TF_Tensor), num_values : c_int, ref status : TF_Status) : void;

    extern proc TF_SetAttrTensorList(desc : c_ptr(TF_OperationDescription), attr_name : c_string, values : c_ptr(c_ptr(TF_Tensor)), num_values : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_SetAttrValueProto(ref desc : TF_OperationDescription, attr_name : c_string, proto : c_void_ptr, proto_len : size_t, ref status : TF_Status) : void;

    extern proc TF_SetAttrValueProto(desc : c_ptr(TF_OperationDescription), attr_name : c_string, proto : c_void_ptr, proto_len : size_t, status : c_ptr(TF_Status)) : void;

    extern proc TF_FinishOperation(ref desc : TF_OperationDescription, ref status : TF_Status) : c_ptr(TF_Operation);

    extern proc TF_FinishOperation(desc : c_ptr(TF_OperationDescription), status : c_ptr(TF_Status)) : c_ptr(TF_Operation);

    extern proc TF_OperationName(ref oper : TF_Operation) : c_string;

    extern proc TF_OperationName(oper : c_ptr(TF_Operation)) : c_string;

    extern proc TF_OperationOpType(ref oper : TF_Operation) : c_string;

    extern proc TF_OperationOpType(oper : c_ptr(TF_Operation)) : c_string;

    extern proc TF_OperationDevice(ref oper : TF_Operation) : c_string;

    extern proc TF_OperationDevice(oper : c_ptr(TF_Operation)) : c_string;

    extern proc TF_OperationNumOutputs(ref oper : TF_Operation) : c_int;

    extern proc TF_OperationNumOutputs(oper : c_ptr(TF_Operation)) : c_int;

    extern proc TF_OperationOutputType(oper_out : TF_Output) : TF_DataType;

    extern proc TF_OperationOutputListLength(ref oper : TF_Operation, arg_name : c_string, ref status : TF_Status) : c_int;

    extern proc TF_OperationOutputListLength(oper : c_ptr(TF_Operation), arg_name : c_string, status : c_ptr(TF_Status)) : c_int;

    extern proc TF_OperationNumInputs(ref oper : TF_Operation) : c_int;

    extern proc TF_OperationNumInputs(oper : c_ptr(TF_Operation)) : c_int;

    extern proc TF_OperationInputType(oper_in : TF_Input) : TF_DataType;

    extern proc TF_OperationInputListLength(ref oper : TF_Operation, arg_name : c_string, ref status : TF_Status) : c_int;

    extern proc TF_OperationInputListLength(oper : c_ptr(TF_Operation), arg_name : c_string, status : c_ptr(TF_Status)) : c_int;

    extern proc TF_OperationInput(oper_in : TF_Input) : TF_Output;

    extern proc TF_OperationOutputNumConsumers(oper_out : TF_Output) : c_int;

    extern proc TF_OperationOutputConsumers(oper_out : TF_Output, ref consumers : TF_Input, max_consumers : c_int) : c_int;

    extern proc TF_OperationOutputConsumers(oper_out : TF_Output, consumers : c_ptr(TF_Input), max_consumers : c_int) : c_int;

    extern proc TF_OperationNumControlInputs(ref oper : TF_Operation) : c_int;

    extern proc TF_OperationNumControlInputs(oper : c_ptr(TF_Operation)) : c_int;

    extern proc TF_OperationGetControlInputs(ref oper : TF_Operation, ref control_inputs : c_ptr(TF_Operation), max_control_inputs : c_int) : c_int;

    extern proc TF_OperationGetControlInputs(oper : c_ptr(TF_Operation), control_inputs : c_ptr(c_ptr(TF_Operation)), max_control_inputs : c_int) : c_int;

    extern proc TF_OperationNumControlOutputs(ref oper : TF_Operation) : c_int;

    extern proc TF_OperationNumControlOutputs(oper : c_ptr(TF_Operation)) : c_int;

    extern proc TF_OperationGetControlOutputs(ref oper : TF_Operation, ref control_outputs : c_ptr(TF_Operation), max_control_outputs : c_int) : c_int;

    extern proc TF_OperationGetControlOutputs(oper : c_ptr(TF_Operation), control_outputs : c_ptr(c_ptr(TF_Operation)), max_control_outputs : c_int) : c_int;

    extern proc TF_OperationGetAttrMetadata(ref oper : TF_Operation, attr_name : c_string, ref status : TF_Status) : TF_AttrMetadata;

    extern proc TF_OperationGetAttrMetadata(oper : c_ptr(TF_Operation), attr_name : c_string, status : c_ptr(TF_Status)) : TF_AttrMetadata;

    extern proc TF_OperationGetAttrString(ref oper : TF_Operation, attr_name : c_string, value : c_void_ptr, max_length : size_t, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrString(oper : c_ptr(TF_Operation), attr_name : c_string, value : c_void_ptr, max_length : size_t, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrStringList(ref oper : TF_Operation, attr_name : c_string, ref values : c_void_ptr, ref lengths : size_t, max_values : c_int, storage : c_void_ptr, storage_size : size_t, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrStringList(oper : c_ptr(TF_Operation), attr_name : c_string, values : c_ptr(c_void_ptr), lengths : c_ptr(size_t), max_values : c_int, storage : c_void_ptr, storage_size : size_t, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrInt(ref oper : TF_Operation, attr_name : c_string, ref value : int(64), ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrInt(oper : c_ptr(TF_Operation), attr_name : c_string, value : c_ptr(int(64)), status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrIntList(ref oper : TF_Operation, attr_name : c_string, ref values : int(64), max_values : c_int, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrIntList(oper : c_ptr(TF_Operation), attr_name : c_string, values : c_ptr(int(64)), max_values : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrFloat(ref oper : TF_Operation, attr_name : c_string, ref value : c_float, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrFloat(oper : c_ptr(TF_Operation), attr_name : c_string, value : c_ptr(c_float), status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrFloatList(ref oper : TF_Operation, attr_name : c_string, ref values : c_float, max_values : c_int, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrFloatList(oper : c_ptr(TF_Operation), attr_name : c_string, values : c_ptr(c_float), max_values : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrBool(ref oper : TF_Operation, attr_name : c_string, ref value : c_uchar, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrBool(oper : c_ptr(TF_Operation), attr_name : c_string, value : c_ptr(c_uchar), status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrBoolList(ref oper : TF_Operation, attr_name : c_string, ref values : c_uchar, max_values : c_int, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrBoolList(oper : c_ptr(TF_Operation), attr_name : c_string, values : c_ptr(c_uchar), max_values : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrType(ref oper : TF_Operation, attr_name : c_string, ref value : TF_DataType, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrType(oper : c_ptr(TF_Operation), attr_name : c_string, value : c_ptr(TF_DataType), status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrTypeList(ref oper : TF_Operation, attr_name : c_string, ref values : TF_DataType, max_values : c_int, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrTypeList(oper : c_ptr(TF_Operation), attr_name : c_string, values : c_ptr(TF_DataType), max_values : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrShape(ref oper : TF_Operation, attr_name : c_string, ref value : int(64), num_dims : c_int, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrShape(oper : c_ptr(TF_Operation), attr_name : c_string, value : c_ptr(int(64)), num_dims : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrShapeList(ref oper : TF_Operation, attr_name : c_string, ref dims : c_ptr(int(64)), ref num_dims : c_int, num_shapes : c_int, ref storage : int(64), storage_size : c_int, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrShapeList(oper : c_ptr(TF_Operation), attr_name : c_string, dims : c_ptr(c_ptr(int(64))), num_dims : c_ptr(c_int), num_shapes : c_int, storage : c_ptr(int(64)), storage_size : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrTensorShapeProto(ref oper : TF_Operation, attr_name : c_string, ref value : TF_Buffer, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrTensorShapeProto(oper : c_ptr(TF_Operation), attr_name : c_string, value : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrTensorShapeProtoList(ref oper : TF_Operation, attr_name : c_string, ref values : c_ptr(TF_Buffer), max_values : c_int, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrTensorShapeProtoList(oper : c_ptr(TF_Operation), attr_name : c_string, values : c_ptr(c_ptr(TF_Buffer)), max_values : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrTensor(ref oper : TF_Operation, attr_name : c_string, ref value : c_ptr(TF_Tensor), ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrTensor(oper : c_ptr(TF_Operation), attr_name : c_string, value : c_ptr(c_ptr(TF_Tensor)), status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrTensorList(ref oper : TF_Operation, attr_name : c_string, ref values : c_ptr(TF_Tensor), max_values : c_int, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrTensorList(oper : c_ptr(TF_Operation), attr_name : c_string, values : c_ptr(c_ptr(TF_Tensor)), max_values : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_OperationGetAttrValueProto(ref oper : TF_Operation, attr_name : c_string, ref output_attr_value : TF_Buffer, ref status : TF_Status) : void;

    extern proc TF_OperationGetAttrValueProto(oper : c_ptr(TF_Operation), attr_name : c_string, output_attr_value : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : void;

    extern proc TF_GraphOperationByName(ref graph : TF_Graph, oper_name : c_string) : c_ptr(TF_Operation);

    extern proc TF_GraphOperationByName(graph : c_ptr(TF_Graph), oper_name : c_string) : c_ptr(TF_Operation);

    extern proc TF_GraphNextOperation(ref graph : TF_Graph, ref pos : size_t) : c_ptr(TF_Operation);

    extern proc TF_GraphNextOperation(graph : c_ptr(TF_Graph), pos : c_ptr(size_t)) : c_ptr(TF_Operation);

    extern proc TF_GraphToGraphDef(ref graph : TF_Graph, ref output_graph_def : TF_Buffer, ref status : TF_Status) : void;

    extern proc TF_GraphToGraphDef(graph : c_ptr(TF_Graph), output_graph_def : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : void;

    extern proc TF_GraphGetOpDef(ref graph : TF_Graph, op_name : c_string, ref output_op_def : TF_Buffer, ref status : TF_Status) : void;

    extern proc TF_GraphGetOpDef(graph : c_ptr(TF_Graph), op_name : c_string, output_op_def : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : void;

    extern proc TF_GraphVersions(ref graph : TF_Graph, ref output_version_def : TF_Buffer, ref status : TF_Status) : void;

    extern proc TF_GraphVersions(graph : c_ptr(TF_Graph), output_version_def : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : void;

    extern proc TF_NewImportGraphDefOptions() : c_ptr(TF_ImportGraphDefOptions);

    extern proc TF_DeleteImportGraphDefOptions(ref opts : TF_ImportGraphDefOptions) : void;

    extern proc TF_DeleteImportGraphDefOptions(opts : c_ptr(TF_ImportGraphDefOptions)) : void;

    extern proc TF_ImportGraphDefOptionsSetPrefix(ref opts : TF_ImportGraphDefOptions, prefix : c_string) : void;

    extern proc TF_ImportGraphDefOptionsSetPrefix(opts : c_ptr(TF_ImportGraphDefOptions), prefix : c_string) : void;

    extern proc TF_ImportGraphDefOptionsSetUniquifyNames(ref opts : TF_ImportGraphDefOptions, uniquify_names : c_uchar) : void;

    extern proc TF_ImportGraphDefOptionsSetUniquifyNames(opts : c_ptr(TF_ImportGraphDefOptions), uniquify_names : c_uchar) : void;

    extern proc TF_ImportGraphDefOptionsSetUniquifyPrefix(ref opts : TF_ImportGraphDefOptions, uniquify_prefix : c_uchar) : void;

    extern proc TF_ImportGraphDefOptionsSetUniquifyPrefix(opts : c_ptr(TF_ImportGraphDefOptions), uniquify_prefix : c_uchar) : void;

    extern proc TF_ImportGraphDefOptionsAddInputMapping(ref opts : TF_ImportGraphDefOptions, src_name : c_string, src_index : c_int, dst : TF_Output) : void;

    extern proc TF_ImportGraphDefOptionsAddInputMapping(opts : c_ptr(TF_ImportGraphDefOptions), src_name : c_string, src_index : c_int, dst : TF_Output) : void;

    extern proc TF_ImportGraphDefOptionsRemapControlDependency(ref opts : TF_ImportGraphDefOptions, src_name : c_string, ref dst : TF_Operation) : void;

    extern proc TF_ImportGraphDefOptionsRemapControlDependency(opts : c_ptr(TF_ImportGraphDefOptions), src_name : c_string, dst : c_ptr(TF_Operation)) : void;

    extern proc TF_ImportGraphDefOptionsAddControlDependency(ref opts : TF_ImportGraphDefOptions, ref oper : TF_Operation) : void;

    extern proc TF_ImportGraphDefOptionsAddControlDependency(opts : c_ptr(TF_ImportGraphDefOptions), oper : c_ptr(TF_Operation)) : void;

    extern proc TF_ImportGraphDefOptionsAddReturnOutput(ref opts : TF_ImportGraphDefOptions, oper_name : c_string, index_arg : c_int) : void;

    extern proc TF_ImportGraphDefOptionsAddReturnOutput(opts : c_ptr(TF_ImportGraphDefOptions), oper_name : c_string, index_arg : c_int) : void;

    extern proc TF_ImportGraphDefOptionsNumReturnOutputs(ref opts : TF_ImportGraphDefOptions) : c_int;

    extern proc TF_ImportGraphDefOptionsNumReturnOutputs(opts : c_ptr(TF_ImportGraphDefOptions)) : c_int;

    extern proc TF_ImportGraphDefOptionsAddReturnOperation(ref opts : TF_ImportGraphDefOptions, oper_name : c_string) : void;

    extern proc TF_ImportGraphDefOptionsAddReturnOperation(opts : c_ptr(TF_ImportGraphDefOptions), oper_name : c_string) : void;

    extern proc TF_ImportGraphDefOptionsNumReturnOperations(ref opts : TF_ImportGraphDefOptions) : c_int;

    extern proc TF_ImportGraphDefOptionsNumReturnOperations(opts : c_ptr(TF_ImportGraphDefOptions)) : c_int;

    extern proc TF_ImportGraphDefResultsReturnOutputs(ref results : TF_ImportGraphDefResults, ref num_outputs : c_int, ref outputs : c_ptr(TF_Output)) : void;

    extern proc TF_ImportGraphDefResultsReturnOutputs(results : c_ptr(TF_ImportGraphDefResults), num_outputs : c_ptr(c_int), outputs : c_ptr(c_ptr(TF_Output))) : void;

    extern proc TF_ImportGraphDefResultsReturnOperations(ref results : TF_ImportGraphDefResults, ref num_opers : c_int, ref opers : c_ptr(c_ptr(TF_Operation))) : void;

    extern proc TF_ImportGraphDefResultsReturnOperations(results : c_ptr(TF_ImportGraphDefResults), num_opers : c_ptr(c_int), opers : c_ptr(c_ptr(c_ptr(TF_Operation)))) : void;

    extern proc TF_ImportGraphDefResultsMissingUnusedInputMappings(ref results : TF_ImportGraphDefResults, ref num_missing_unused_input_mappings : c_int, ref src_names : c_ptr(c_string), ref src_indexes : c_ptr(c_int)) : void;

    extern proc TF_ImportGraphDefResultsMissingUnusedInputMappings(results : c_ptr(TF_ImportGraphDefResults), num_missing_unused_input_mappings : c_ptr(c_int), src_names : c_ptr(c_ptr(c_string)), src_indexes : c_ptr(c_ptr(c_int))) : void;

    extern proc TF_DeleteImportGraphDefResults(ref results : TF_ImportGraphDefResults) : void;

    extern proc TF_DeleteImportGraphDefResults(results : c_ptr(TF_ImportGraphDefResults)) : void;

    extern proc TF_GraphImportGraphDefWithResults(ref graph : TF_Graph, ref graph_def : TF_Buffer, ref options : TF_ImportGraphDefOptions, ref status : TF_Status) : c_ptr(TF_ImportGraphDefResults);

    extern proc TF_GraphImportGraphDefWithResults(graph : c_ptr(TF_Graph), graph_def : c_ptr(TF_Buffer), options : c_ptr(TF_ImportGraphDefOptions), status : c_ptr(TF_Status)) : c_ptr(TF_ImportGraphDefResults);

    extern proc TF_GraphImportGraphDefWithReturnOutputs(ref graph : TF_Graph, ref graph_def : TF_Buffer, ref options : TF_ImportGraphDefOptions, ref return_outputs : TF_Output, num_return_outputs : c_int, ref status : TF_Status) : void;

    extern proc TF_GraphImportGraphDefWithReturnOutputs(graph : c_ptr(TF_Graph), graph_def : c_ptr(TF_Buffer), options : c_ptr(TF_ImportGraphDefOptions), return_outputs : c_ptr(TF_Output), num_return_outputs : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_GraphImportGraphDef(ref graph : TF_Graph, ref graph_def : TF_Buffer, ref options : TF_ImportGraphDefOptions, ref status : TF_Status) : void;

    extern proc TF_GraphImportGraphDef(graph : c_ptr(TF_Graph), graph_def : c_ptr(TF_Buffer), options : c_ptr(TF_ImportGraphDefOptions), status : c_ptr(TF_Status)) : void;

    extern proc TF_GraphCopyFunction(ref g : TF_Graph, ref func : TF_Function, ref grad : TF_Function, ref status : TF_Status) : void;

    extern proc TF_GraphCopyFunction(g : c_ptr(TF_Graph), func : c_ptr(TF_Function), grad : c_ptr(TF_Function), status : c_ptr(TF_Status)) : void;

    extern proc TF_GraphNumFunctions(ref g : TF_Graph) : c_int;

    extern proc TF_GraphNumFunctions(g : c_ptr(TF_Graph)) : c_int;

    extern proc TF_GraphGetFunctions(ref g : TF_Graph, ref funcs : c_ptr(TF_Function), max_func : c_int, ref status : TF_Status) : c_int;

    extern proc TF_GraphGetFunctions(g : c_ptr(TF_Graph), funcs : c_ptr(c_ptr(TF_Function)), max_func : c_int, status : c_ptr(TF_Status)) : c_int;

    extern proc TF_OperationToNodeDef(ref oper : TF_Operation, ref output_node_def : TF_Buffer, ref status : TF_Status) : void;

    extern proc TF_OperationToNodeDef(oper : c_ptr(TF_Operation), output_node_def : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : void;

    extern proc TF_NewWhile(ref g : TF_Graph, ref inputs : TF_Output, ninputs : c_int, ref status : TF_Status) : TF_WhileParams;

    extern proc TF_NewWhile(g : c_ptr(TF_Graph), inputs : c_ptr(TF_Output), ninputs : c_int, status : c_ptr(TF_Status)) : TF_WhileParams;

    extern proc TF_FinishWhile(ref params : TF_WhileParams, ref status : TF_Status, ref outputs : TF_Output) : void;

    extern proc TF_FinishWhile(params : c_ptr(TF_WhileParams), status : c_ptr(TF_Status), outputs : c_ptr(TF_Output)) : void;

    extern proc TF_AbortWhile(ref params : TF_WhileParams) : void;

    extern proc TF_AbortWhile(params : c_ptr(TF_WhileParams)) : void;

    extern proc TF_AddGradients(ref g : TF_Graph, ref y : TF_Output, ny : c_int, ref x : TF_Output, nx : c_int, ref dx : TF_Output, ref status : TF_Status, ref dy : TF_Output) : void;

    extern proc TF_AddGradients(g : c_ptr(TF_Graph), y : c_ptr(TF_Output), ny : c_int, x : c_ptr(TF_Output), nx : c_int, dx : c_ptr(TF_Output), status : c_ptr(TF_Status), dy : c_ptr(TF_Output)) : void;

    extern proc TF_AddGradientsWithPrefix(ref g : TF_Graph, prefix : c_string, ref y : TF_Output, ny : c_int, ref x : TF_Output, nx : c_int, ref dx : TF_Output, ref status : TF_Status, ref dy : TF_Output) : void;

    extern proc TF_AddGradientsWithPrefix(g : c_ptr(TF_Graph), prefix : c_string, y : c_ptr(TF_Output), ny : c_int, x : c_ptr(TF_Output), nx : c_int, dx : c_ptr(TF_Output), status : c_ptr(TF_Status), dy : c_ptr(TF_Output)) : void;

    extern proc TF_GraphToFunction(ref fn_body : TF_Graph, fn_name : c_string, append_hash_to_fn_name : c_uchar, num_opers : c_int, ref opers : c_ptr(TF_Operation), ninputs : c_int, ref inputs : TF_Output, noutputs : c_int, ref outputs : TF_Output, ref output_names : c_string, ref opts : TF_FunctionOptions, description : c_string, ref status : TF_Status) : c_ptr(TF_Function);

    extern proc TF_GraphToFunction(fn_body : c_ptr(TF_Graph), fn_name : c_string, append_hash_to_fn_name : c_uchar, num_opers : c_int, opers : c_ptr(c_ptr(TF_Operation)), ninputs : c_int, inputs : c_ptr(TF_Output), noutputs : c_int, outputs : c_ptr(TF_Output), output_names : c_ptr(c_string), opts : c_ptr(TF_FunctionOptions), description : c_string, status : c_ptr(TF_Status)) : c_ptr(TF_Function);

    extern proc TF_FunctionName(ref func : TF_Function) : c_string;

    extern proc TF_FunctionName(func : c_ptr(TF_Function)) : c_string;

    extern proc TF_FunctionToFunctionDef(ref func : TF_Function, ref output_func_def : TF_Buffer, ref status : TF_Status) : void;

    extern proc TF_FunctionToFunctionDef(func : c_ptr(TF_Function), output_func_def : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : void;

    extern proc TF_FunctionImportFunctionDef(proto : c_void_ptr, proto_len : size_t, ref status : TF_Status) : c_ptr(TF_Function);

    extern proc TF_FunctionImportFunctionDef(proto : c_void_ptr, proto_len : size_t, status : c_ptr(TF_Status)) : c_ptr(TF_Function);

    extern proc TF_FunctionSetAttrValueProto(ref func : TF_Function, attr_name : c_string, proto : c_void_ptr, proto_len : size_t, ref status : TF_Status) : void;

    extern proc TF_FunctionSetAttrValueProto(func : c_ptr(TF_Function), attr_name : c_string, proto : c_void_ptr, proto_len : size_t, status : c_ptr(TF_Status)) : void;

    extern proc TF_FunctionGetAttrValueProto(ref func : TF_Function, attr_name : c_string, ref output_attr_value : TF_Buffer, ref status : TF_Status) : void;

    extern proc TF_FunctionGetAttrValueProto(func : c_ptr(TF_Function), attr_name : c_string, output_attr_value : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : void;

    extern proc TF_DeleteFunction(ref func : TF_Function) : void;

    extern proc TF_DeleteFunction(func : c_ptr(TF_Function)) : void;

    extern proc TF_TryEvaluateConstant(ref graph : TF_Graph, output : TF_Output, ref result : c_ptr(TF_Tensor), ref status : TF_Status) : c_uchar;

    extern proc TF_TryEvaluateConstant(graph : c_ptr(TF_Graph), output : TF_Output, result : c_ptr(c_ptr(TF_Tensor)), status : c_ptr(TF_Status)) : c_uchar;

    extern proc TF_NewSession(ref graph : TF_Graph, ref opts : TF_SessionOptions, ref status : TF_Status) : c_ptr(TF_Session);

    extern proc TF_NewSession(graph : c_ptr(TF_Graph), opts : c_ptr(TF_SessionOptions), status : c_ptr(TF_Status)) : c_ptr(TF_Session);

    extern proc TF_LoadSessionFromSavedModel(ref session_options : TF_SessionOptions, ref run_options : TF_Buffer, export_dir : c_string, ref tags : c_string, tags_len : c_int, ref graph : TF_Graph, ref meta_graph_def : TF_Buffer, ref status : TF_Status) : c_ptr(TF_Session);

    extern proc TF_LoadSessionFromSavedModel(session_options : c_ptr(TF_SessionOptions), run_options : c_ptr(TF_Buffer), export_dir : c_string, tags : c_ptr(c_string), tags_len : c_int, graph : c_ptr(TF_Graph), meta_graph_def : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : c_ptr(TF_Session);

    extern proc TF_CloseSession(ref arg0 : TF_Session, ref status : TF_Status) : void;

    extern proc TF_CloseSession(arg0 : c_ptr(TF_Session), status : c_ptr(TF_Status)) : void;

    extern proc TF_DeleteSession(ref arg0 : TF_Session, ref status : TF_Status) : void;

    extern proc TF_DeleteSession(arg0 : c_ptr(TF_Session), status : c_ptr(TF_Status)) : void;

    extern proc TF_SessionRun(ref session : TF_Session, ref run_options : TF_Buffer, ref inputs : TF_Output, ref input_values : c_ptr(TF_Tensor), ninputs : c_int, ref outputs : TF_Output, ref output_values : c_ptr(TF_Tensor), noutputs : c_int, ref target_opers : c_ptr(TF_Operation), ntargets : c_int, ref run_metadata : TF_Buffer, ref arg11 : TF_Status) : void;

    extern proc TF_SessionRun(session : c_ptr(TF_Session), run_options : c_ptr(TF_Buffer), inputs : c_ptr(TF_Output), input_values : c_ptr(c_ptr(TF_Tensor)), ninputs : c_int, outputs : c_ptr(TF_Output), output_values : c_ptr(c_ptr(TF_Tensor)), noutputs : c_int, target_opers : c_ptr(c_ptr(TF_Operation)), ntargets : c_int, run_metadata : c_ptr(TF_Buffer), arg11 : c_ptr(TF_Status)) : void;

    extern proc TF_SessionPRunSetup(ref arg0 : TF_Session, ref inputs : TF_Output, ninputs : c_int, ref outputs : TF_Output, noutputs : c_int, ref target_opers : c_ptr(TF_Operation), ntargets : c_int, ref handle : c_string, ref arg8 : TF_Status) : void;

    extern proc TF_SessionPRunSetup(arg0 : c_ptr(TF_Session), inputs : c_ptr(TF_Output), ninputs : c_int, outputs : c_ptr(TF_Output), noutputs : c_int, target_opers : c_ptr(c_ptr(TF_Operation)), ntargets : c_int, handle : c_ptr(c_string), arg8 : c_ptr(TF_Status)) : void;

    extern proc TF_SessionPRun(ref arg0 : TF_Session, handle : c_string, ref inputs : TF_Output, ref input_values : c_ptr(TF_Tensor), ninputs : c_int, ref outputs : TF_Output, ref output_values : c_ptr(TF_Tensor), noutputs : c_int, ref target_opers : c_ptr(TF_Operation), ntargets : c_int, ref arg10 : TF_Status) : void;

    extern proc TF_SessionPRun(arg0 : c_ptr(TF_Session), handle : c_string, inputs : c_ptr(TF_Output), input_values : c_ptr(c_ptr(TF_Tensor)), ninputs : c_int, outputs : c_ptr(TF_Output), output_values : c_ptr(c_ptr(TF_Tensor)), noutputs : c_int, target_opers : c_ptr(c_ptr(TF_Operation)), ntargets : c_int, arg10 : c_ptr(TF_Status)) : void;

    extern proc TF_DeletePRunHandle(handle : c_string) : void;

    extern proc TF_NewDeprecatedSession(ref arg0 : TF_SessionOptions, ref status : TF_Status) : c_ptr(TF_DeprecatedSession);

    extern proc TF_NewDeprecatedSession(arg0 : c_ptr(TF_SessionOptions), status : c_ptr(TF_Status)) : c_ptr(TF_DeprecatedSession);

    extern proc TF_CloseDeprecatedSession(ref arg0 : TF_DeprecatedSession, ref status : TF_Status) : void;

    extern proc TF_CloseDeprecatedSession(arg0 : c_ptr(TF_DeprecatedSession), status : c_ptr(TF_Status)) : void;

    extern proc TF_DeleteDeprecatedSession(ref arg0 : TF_DeprecatedSession, ref status : TF_Status) : void;

    extern proc TF_DeleteDeprecatedSession(arg0 : c_ptr(TF_DeprecatedSession), status : c_ptr(TF_Status)) : void;

    extern proc TF_Reset(ref opt : TF_SessionOptions, ref containers : c_string, ncontainers : c_int, ref status : TF_Status) : void;

    extern proc TF_Reset(opt : c_ptr(TF_SessionOptions), containers : c_ptr(c_string), ncontainers : c_int, status : c_ptr(TF_Status)) : void;

    extern proc TF_ExtendGraph(ref arg0 : TF_DeprecatedSession, proto : c_void_ptr, proto_len : size_t, ref arg3 : TF_Status) : void;

    extern proc TF_ExtendGraph(arg0 : c_ptr(TF_DeprecatedSession), proto : c_void_ptr, proto_len : size_t, arg3 : c_ptr(TF_Status)) : void;

    extern proc TF_Run(ref arg0 : TF_DeprecatedSession, ref run_options : TF_Buffer, ref input_names : c_string, ref inputs : c_ptr(TF_Tensor), ninputs : c_int, ref output_names : c_string, ref outputs : c_ptr(TF_Tensor), noutputs : c_int, ref target_oper_names : c_string, ntargets : c_int, ref run_metadata : TF_Buffer, ref arg11 : TF_Status) : void;

    extern proc TF_Run(arg0 : c_ptr(TF_DeprecatedSession), run_options : c_ptr(TF_Buffer), input_names : c_ptr(c_string), inputs : c_ptr(c_ptr(TF_Tensor)), ninputs : c_int, output_names : c_ptr(c_string), outputs : c_ptr(c_ptr(TF_Tensor)), noutputs : c_int, target_oper_names : c_ptr(c_string), ntargets : c_int, run_metadata : c_ptr(TF_Buffer), arg11 : c_ptr(TF_Status)) : void;

    extern proc TF_PRunSetup(ref arg0 : TF_DeprecatedSession, ref input_names : c_string, ninputs : c_int, ref output_names : c_string, noutputs : c_int, ref target_oper_names : c_string, ntargets : c_int, ref handle : c_string, ref arg8 : TF_Status) : void;

    extern proc TF_PRunSetup(arg0 : c_ptr(TF_DeprecatedSession), input_names : c_ptr(c_string), ninputs : c_int, output_names : c_ptr(c_string), noutputs : c_int, target_oper_names : c_ptr(c_string), ntargets : c_int, handle : c_ptr(c_string), arg8 : c_ptr(TF_Status)) : void;

    extern proc TF_PRun(ref arg0 : TF_DeprecatedSession, handle : c_string, ref input_names : c_string, ref inputs : c_ptr(TF_Tensor), ninputs : c_int, ref output_names : c_string, ref outputs : c_ptr(TF_Tensor), noutputs : c_int, ref target_oper_names : c_string, ntargets : c_int, ref arg10 : TF_Status) : void;

    extern proc TF_PRun(arg0 : c_ptr(TF_DeprecatedSession), handle : c_string, input_names : c_ptr(c_string), inputs : c_ptr(c_ptr(TF_Tensor)), ninputs : c_int, output_names : c_ptr(c_string), outputs : c_ptr(c_ptr(TF_Tensor)), noutputs : c_int, target_oper_names : c_ptr(c_string), ntargets : c_int, arg10 : c_ptr(TF_Status)) : void;

    extern proc TF_SessionListDevices(ref session : TF_Session, ref status : TF_Status) : c_ptr(TF_DeviceList);

    extern proc TF_SessionListDevices(session : c_ptr(TF_Session), status : c_ptr(TF_Status)) : c_ptr(TF_DeviceList);

    extern proc TF_DeprecatedSessionListDevices(ref session : TF_DeprecatedSession, ref status : TF_Status) : c_ptr(TF_DeviceList);

    extern proc TF_DeprecatedSessionListDevices(session : c_ptr(TF_DeprecatedSession), status : c_ptr(TF_Status)) : c_ptr(TF_DeviceList);

    extern proc TF_DeleteDeviceList(ref list : TF_DeviceList) : void;

    extern proc TF_DeleteDeviceList(list : c_ptr(TF_DeviceList)) : void;

    extern proc TF_DeviceListCount(ref list : TF_DeviceList) : c_int;

    extern proc TF_DeviceListCount(list : c_ptr(TF_DeviceList)) : c_int;

    extern proc TF_DeviceListName(ref list : TF_DeviceList, index_arg : c_int, ref status : TF_Status) : c_string;

    extern proc TF_DeviceListName(list : c_ptr(TF_DeviceList), index_arg : c_int, status : c_ptr(TF_Status)) : c_string;

    extern proc TF_DeviceListType(ref list : TF_DeviceList, index_arg : c_int, ref status : TF_Status) : c_string;

    extern proc TF_DeviceListType(list : c_ptr(TF_DeviceList), index_arg : c_int, status : c_ptr(TF_Status)) : c_string;

    extern proc TF_DeviceListMemoryBytes(ref list : TF_DeviceList, index_arg : c_int, ref status : TF_Status) : int(64);

    extern proc TF_DeviceListMemoryBytes(list : c_ptr(TF_DeviceList), index_arg : c_int, status : c_ptr(TF_Status)) : int(64);

    extern proc TF_DeviceListIncarnation(ref list : TF_DeviceList, index_arg : c_int, ref status : TF_Status) : uint(64);

    extern proc TF_DeviceListIncarnation(list : c_ptr(TF_DeviceList), index_arg : c_int, status : c_ptr(TF_Status)) : uint(64);

    extern proc TF_LoadLibrary(library_filename : c_string, ref status : TF_Status) : c_ptr(TF_Library);

    extern proc TF_LoadLibrary(library_filename : c_string, status : c_ptr(TF_Status)) : c_ptr(TF_Library);

    extern proc TF_GetOpList(ref lib_handle : TF_Library) : TF_Buffer;

    extern proc TF_GetOpList(lib_handle : c_ptr(TF_Library)) : TF_Buffer;

    extern proc TF_DeleteLibraryHandle(ref lib_handle : TF_Library) : void;

    extern proc TF_DeleteLibraryHandle(lib_handle : c_ptr(TF_Library)) : void;

    extern proc TF_GetAllOpList() : c_ptr(TF_Buffer);

    extern proc TF_NewApiDefMap(ref op_list_buffer : TF_Buffer, ref status : TF_Status) : c_ptr(TF_ApiDefMap);

    extern proc TF_NewApiDefMap(op_list_buffer : c_ptr(TF_Buffer), status : c_ptr(TF_Status)) : c_ptr(TF_ApiDefMap);

    extern proc TF_DeleteApiDefMap(ref apimap : TF_ApiDefMap) : void;

    extern proc TF_DeleteApiDefMap(apimap : c_ptr(TF_ApiDefMap)) : void;

    extern proc TF_ApiDefMapPut(ref api_def_map : TF_ApiDefMap, text : c_string, text_len : size_t, ref status : TF_Status) : void;

    extern proc TF_ApiDefMapPut(api_def_map : c_ptr(TF_ApiDefMap), text : c_string, text_len : size_t, status : c_ptr(TF_Status)) : void;

    extern proc TF_ApiDefMapGet(ref api_def_map : TF_ApiDefMap, name : c_string, name_len : size_t, ref status : TF_Status) : c_ptr(TF_Buffer);

    extern proc TF_ApiDefMapGet(api_def_map : c_ptr(TF_ApiDefMap), name : c_string, name_len : size_t, status : c_ptr(TF_Status)) : c_ptr(TF_Buffer);

    extern proc TF_GetAllRegisteredKernels(ref status : TF_Status) : c_ptr(TF_Buffer);

    extern proc TF_GetAllRegisteredKernels(status : c_ptr(TF_Status)) : c_ptr(TF_Buffer);

    extern proc TF_GetRegisteredKernelsForOp(name : c_string, ref status : TF_Status) : c_ptr(TF_Buffer);

    extern proc TF_GetRegisteredKernelsForOp(name : c_string, status : c_ptr(TF_Status)) : c_ptr(TF_Buffer);

    // ==== c2chapel typedefs ====

    // Opaque struct?
    extern record TF_ApiDefMap {};

    // Fields omitted because one or more of the identifiers is a Chapel keyword
    extern record TF_AttrMetadata {}

    // TF_AttrType enum
    extern type TF_AttrType = c_int;
    extern const TF_ATTR_STRING :TF_AttrType;
    extern const TF_ATTR_INT :TF_AttrType;
    extern const TF_ATTR_FLOAT :TF_AttrType;
    extern const TF_ATTR_BOOL :TF_AttrType;
    extern const TF_ATTR_TYPE :TF_AttrType;
    extern const TF_ATTR_SHAPE :TF_AttrType;
    extern const TF_ATTR_TENSOR :TF_AttrType;
    extern const TF_ATTR_PLACEHOLDER :TF_AttrType;
    extern const TF_ATTR_FUNC :TF_AttrType;


    extern record TF_Buffer {
      var data : c_void_ptr;
      var length : size_t;
      var data_deallocator : c_fn_ptr;
    }

    // TF_Code enum
    extern type TF_Code = c_int;
    extern const TF_OK :TF_Code;
    extern const TF_CANCELLED :TF_Code;
    extern const TF_UNKNOWN :TF_Code;
    extern const TF_INVALID_ARGUMENT :TF_Code;
    extern const TF_DEADLINE_EXCEEDED :TF_Code;
    extern const TF_NOT_FOUND :TF_Code;
    extern const TF_ALREADY_EXISTS :TF_Code;
    extern const TF_PERMISSION_DENIED :TF_Code;
    extern const TF_UNAUTHENTICATED :TF_Code;
    extern const TF_RESOURCE_EXHAUSTED :TF_Code;
    extern const TF_FAILED_PRECONDITION :TF_Code;
    extern const TF_ABORTED :TF_Code;
    extern const TF_OUT_OF_RANGE :TF_Code;
    extern const TF_UNIMPLEMENTED :TF_Code;
    extern const TF_INTERNAL :TF_Code;
    extern const TF_UNAVAILABLE :TF_Code;
    extern const TF_DATA_LOSS :TF_Code;


    // TF_DataType enum
    extern type TF_DataType = c_int;
    extern const TF_FLOAT :TF_DataType;
    extern const TF_DOUBLE :TF_DataType;
    extern const TF_INT32 :TF_DataType;
    extern const TF_UINT8 :TF_DataType;
    extern const TF_INT16 :TF_DataType;
    extern const TF_INT8 :TF_DataType;
    extern const TF_STRING :TF_DataType;
    extern const TF_COMPLEX64 :TF_DataType;
    extern const TF_COMPLEX :TF_DataType;
    extern const TF_INT64 :TF_DataType;
    extern const TF_BOOL :TF_DataType;
    extern const TF_QINT8 :TF_DataType;
    extern const TF_QUINT8 :TF_DataType;
    extern const TF_QINT32 :TF_DataType;
    extern const TF_BFLOAT16 :TF_DataType;
    extern const TF_QINT16 :TF_DataType;
    extern const TF_QUINT16 :TF_DataType;
    extern const TF_UINT16 :TF_DataType;
    extern const TF_COMPLEX128 :TF_DataType;
    extern const TF_HALF :TF_DataType;
    extern const TF_RESOURCE :TF_DataType;
    extern const TF_VARIANT :TF_DataType;
    extern const TF_UINT32 :TF_DataType;
    extern const TF_UINT64 :TF_DataType;


    // Opaque struct?
    extern record TF_DeprecatedSession {};

    // Opaque struct?
    extern record TF_DeviceList {};

    // Opaque struct?
    extern record TF_Function {};

    // Opaque struct?
    extern record TF_FunctionOptions {};

    // Opaque struct?
    extern record TF_Graph {};

    // Opaque struct?
    extern record TF_ImportGraphDefOptions {};

    // Opaque struct?
    extern record TF_ImportGraphDefResults {};

    // Fields omitted because one or more of the identifiers is a Chapel keyword
    extern record TF_Input {}

    // Opaque struct?
    extern record TF_Library {};

    // Opaque struct?
    extern record TF_Operation {};

    // Opaque struct?
    extern record TF_OperationDescription {};

    // Fields omitted because one or more of the identifiers is a Chapel keyword
    extern record TF_Output {}

    // Opaque struct?
    extern record TF_Session {};

    // Opaque struct?
    extern record TF_SessionOptions {};

    // Opaque struct?
    extern record TF_Status {};

    // Opaque struct?
    extern record TF_Tensor {};

    extern record TF_WhileParams {
      var ninputs : c_int;
      var cond_graph : c_ptr(TF_Graph);
      var cond_inputs : c_ptr(TF_Output);
      var cond_output : TF_Output;
      var body_graph : c_ptr(TF_Graph);
      var body_inputs : c_ptr(TF_Output);
      var body_outputs : c_ptr(TF_Output);
      var name : c_string;
    }
  }
}
