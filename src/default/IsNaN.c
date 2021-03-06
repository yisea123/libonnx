#include <onnx.h>

static int IsNaN_init(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x;
	struct onnx_tensor_t * y;

	if((n->ninput > 0) && (n->noutput > 0))
	{
		x = n->inputs[0];
		y = n->outputs[0];
		if(!onnx_tensor_shape_equal(y, x) || (y->type != ONNX_TENSOR_TYPE_BOOL))
			onnx_tensor_reinit(y, ONNX_TENSOR_TYPE_BOOL, x->dims, x->ndim);
		return 1;
	}
	return 0;
}

static int IsNaN_exit(struct onnx_node_t * n)
{
	return 1;
}

static void IsNaN_bfloat16(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x = n->inputs[0];
	struct onnx_tensor_t * y = n->outputs[0];
	uint16_t * px = (uint16_t *)x->datas;
	uint8_t * py = (uint8_t *)y->datas;
	float v;
	int i, l;

	for(i = 0, l = y->ndata; i < l; i++)
	{
		v = bfloat16_to_float32(px[i]);
		py[i] = isnanf(v) ? 1 : 0;
	}
}

static void IsNaN_float16(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x = n->inputs[0];
	struct onnx_tensor_t * y = n->outputs[0];
	uint16_t * px = (uint16_t *)x->datas;
	uint8_t * py = (uint8_t *)y->datas;
	float v;
	int i, l;

	for(i = 0, l = y->ndata; i < l; i++)
	{
		v = float16_to_float32(px[i]);
		py[i] = isnanf(v) ? 1 : 0;
	}
}

static void IsNaN_float32(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x = n->inputs[0];
	struct onnx_tensor_t * y = n->outputs[0];
	float * px = (float *)x->datas;
	uint8_t * py = (uint8_t *)y->datas;
	int i, l;

	for(i = 0, l = y->ndata; i < l; i++)
		py[i] = isnanf(px[i]) ? 1 : 0;
}

static void IsNaN_float64(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x = n->inputs[0];
	struct onnx_tensor_t * y = n->outputs[0];
	double * px = (double *)x->datas;
	uint8_t * py = (uint8_t *)y->datas;
	int i, l;

	for(i = 0, l = y->ndata; i < l; i++)
		py[i] = isnan(px[i]) ? 1 : 0;
}

void resolver_default_op_IsNaN(struct onnx_node_t * n)
{
	switch(n->inputs[0]->type)
	{
	case ONNX_TENSOR_TYPE_BFLOAT16:
		n->init = IsNaN_init;
		n->exit = IsNaN_exit;
		n->op = IsNaN_bfloat16;
		break;
	case ONNX_TENSOR_TYPE_FLOAT16:
		n->init = IsNaN_init;
		n->exit = IsNaN_exit;
		n->op = IsNaN_float16;
		break;
	case ONNX_TENSOR_TYPE_FLOAT32:
		n->init = IsNaN_init;
		n->exit = IsNaN_exit;
		n->op = IsNaN_float32;
		break;
	case ONNX_TENSOR_TYPE_FLOAT64:
		n->init = IsNaN_init;
		n->exit = IsNaN_exit;
		n->op = IsNaN_float64;
		break;
	default:
		break;
	}
}
