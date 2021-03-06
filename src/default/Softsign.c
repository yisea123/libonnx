#include <onnx.h>

static int Softsign_init(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x;
	struct onnx_tensor_t * y;

	if((n->ninput > 0) && (n->noutput > 0))
	{
		x = n->inputs[0];
		y = n->outputs[0];
		if(!onnx_tensor_shape_equal(y, x) || (y->type != x->type))
			onnx_tensor_reinit(y, x->type, x->dims, x->ndim);
		return 1;
	}
	return 0;
}

static int Softsign_exit(struct onnx_node_t * n)
{
	return 1;
}

static void Softsign_float16(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x = n->inputs[0];
	struct onnx_tensor_t * y = n->outputs[0];
	uint16_t * px = (uint16_t *)x->datas;
	uint16_t * py = (uint16_t *)y->datas;
	float v;
	int i, l;

	for(i = 0, l = y->ndata; i < l; i++)
	{
		v = float16_to_float32(px[i]);
		py[i] = float32_to_float16(v / (1 + fabsf(v)));
	}
}

static void Softsign_float32(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x = n->inputs[0];
	struct onnx_tensor_t * y = n->outputs[0];
	float * px = (float *)x->datas;
	float * py = (float *)y->datas;
	int i, l;

	for(i = 0, l = y->ndata; i < l; i++)
		py[i] = px[i] / (1 + fabsf(px[i]));
}

static void Softsign_float64(struct onnx_node_t * n)
{
	struct onnx_tensor_t * x = n->inputs[0];
	struct onnx_tensor_t * y = n->outputs[0];
	double * px = (double *)x->datas;
	double * py = (double *)y->datas;
	int i, l;

	for(i = 0, l = y->ndata; i < l; i++)
		py[i] = px[i] / (1 + fabs(px[i]));
}

void resolver_default_op_Softsign(struct onnx_node_t * n)
{
	switch(n->inputs[0]->type)
	{
	case ONNX_TENSOR_TYPE_FLOAT16:
		n->init = Softsign_init;
		n->exit = Softsign_exit;
		n->op = Softsign_float16;
		break;
	case ONNX_TENSOR_TYPE_FLOAT32:
		n->init = Softsign_init;
		n->exit = Softsign_exit;
		n->op = Softsign_float32;
		break;
	case ONNX_TENSOR_TYPE_FLOAT64:
		n->init = Softsign_init;
		n->exit = Softsign_exit;
		n->op = Softsign_float64;
		break;
	default:
		break;
	}
}
