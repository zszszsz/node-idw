#include <node.h>
#include <v8.h>
#include <math.h>
#include <limits.h>
#include <values.h>
#include <iostream>
#include <uv.h>
using namespace v8;
using namespace std;
namespace idw
{
class arguments
{
  private:
    arguments() {}
  public:
    int width;
    int height;
    int pNum;
    double nth;
    double same;
    double neighbor;
    double loLim;
    double upLim;
    double *pValues;
    double *pLons;
    double *pLats;
    double *lons;
    double *lats;

    arguments(Handle<Array> pointInfo, Handle<Array> lonInfo, Handle<Array> latInfo, Handle<Object> options)
    {
        Isolate *isolate = Isolate::GetCurrent();
        this->nth = options->Has(String::NewFromUtf8(isolate, "nth")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "nth")))->NumberValue() : 1;
        this->same = options->Has(String::NewFromUtf8(isolate, "same")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "same")))->NumberValue() : 2 * DBL_MIN;
        this->neighbor = options->Has(String::NewFromUtf8(isolate, "neighbor")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "neighbor")))->NumberValue() : DBL_MAX;
        this->loLim = options->Has(String::NewFromUtf8(isolate, "loLim")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "loLim")))->NumberValue() : -DBL_MAX;
        this->upLim = options->Has(String::NewFromUtf8(isolate, "upLim")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "upLim")))->NumberValue() : DBL_MAX;

        this->width = lonInfo->Length();
        this->height = latInfo->Length();
        this->pNum = pointInfo->Length();
        this->pValues = (double *)malloc(sizeof(double) * pNum);
        this->pLons = (double *)malloc(sizeof(double) * pNum);
        this->pLats = (double *)malloc(sizeof(double) * pNum);
        this->lons = (double *)malloc(sizeof(double) * width);
        this->lats = (double *)malloc(sizeof(double) * height);
        for (int y = 0; y < height; y++)
        {
            lats[y] = Handle<Number>::Cast(latInfo->Get(y))->NumberValue();
        }
        for (int x = 0; x < width; x++)
        {
            lons[x] = Handle<Number>::Cast(lonInfo->Get(x))->NumberValue();
        }
        for (int p = 0; p < pNum; p++)
        {
            Handle<Object> point = Handle<Object>::Cast(pointInfo->Get(p));
            pValues[p] = Handle<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "value")))->NumberValue();
            pLons[p] = Handle<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "longitude")))->NumberValue();
            pLats[p] = Handle<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "latitude")))->NumberValue();
        }
        return;
    }
    arguments(Handle<Array> pointInfo, Handle<Array> lonInfo, Handle<Array> latInfo)
    {
        Isolate *isolate = Isolate::GetCurrent();
        this->nth = 1;
        this->same = 2 * DBL_MIN;
        this->neighbor = 2 * DBL_MAX;
        this->loLim = -DBL_MAX;
        this->upLim = DBL_MAX;

        this->width = lonInfo->Length();
        this->height = latInfo->Length();
        this->pNum = pointInfo->Length();

        this->pValues = (double *)malloc(sizeof(double) * pNum);
        this->pLons = (double *)malloc(sizeof(double) * pNum);
        this->pLats = (double *)malloc(sizeof(double) * pNum);
        this->lons = (double *)malloc(sizeof(double) * width);
        this->lats = (double *)malloc(sizeof(double) * height);
        for (int y = 0; y < height; y++)
        {
            lats[y] = Handle<Number>::Cast(latInfo->Get(y))->NumberValue();
        }
        for (int x = 0; x < width; x++)
        {
            lons[x] = Handle<Number>::Cast(lonInfo->Get(x))->NumberValue();
        }
        for (int p = 0; p < pNum; p++)
        {
            Handle<Object> point = Handle<Object>::Cast(pointInfo->Get(p));
            pValues[p] = Handle<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "value")))->NumberValue();
            pLons[p] = Handle<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "longitude")))->NumberValue();
            pLats[p] = Handle<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "latitude")))->NumberValue();
        }
        return;
    }
    arguments(Handle<Array> pValueInfo, Handle<Array> pLonInfo, Handle<Array> pLatInfo, Handle<Array> lonInfo, Handle<Array> latInfo, Handle<Object> options)
    {
        Isolate *isolate = Isolate::GetCurrent();
        this->nth = options->Has(String::NewFromUtf8(isolate, "nth")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "nth")))->NumberValue() : 1;
        this->same = options->Has(String::NewFromUtf8(isolate, "same")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "same")))->NumberValue() : 2 * DBL_MIN;
        this->neighbor = options->Has(String::NewFromUtf8(isolate, "neighbor")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "neighbor")))->NumberValue() : DBL_MAX;
        this->loLim = options->Has(String::NewFromUtf8(isolate, "loLim")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "loLim")))->NumberValue() : -DBL_MAX;
        this->upLim = options->Has(String::NewFromUtf8(isolate, "upLim")) ? Local<Number>::Cast(options->Get(String::NewFromUtf8(isolate, "upLim")))->NumberValue() : DBL_MAX;

        width = lonInfo->Length();
        height = latInfo->Length();
        pNum = pValueInfo->Length();

        pValues = (double *)malloc(sizeof(double) * pNum);
        pLons = (double *)malloc(sizeof(double) * pNum);
        pLats = (double *)malloc(sizeof(double) * pNum);
        lons = (double *)malloc(sizeof(double) * width);
        lats = (double *)malloc(sizeof(double) * height);
        for (int y = 0; y < height; y++)
        {
            lats[y] = Handle<Number>::Cast(latInfo->Get(y))->NumberValue();
        }
        for (int x = 0; x < width; x++)
        {
            lons[x] = Handle<Number>::Cast(lonInfo->Get(x))->NumberValue();
        }
        for (int p = 0; p < pNum; p++)
        {
            pValues[p] = Handle<Number>::Cast(pValueInfo->Get(p))->NumberValue();
            pLons[p] = Handle<Number>::Cast(pLonInfo->Get(p))->NumberValue();
            pLats[p] = Handle<Number>::Cast(pLatInfo->Get(p))->NumberValue();
        }

        return;
    }
    arguments(Handle<Array> pValueInfo, Handle<Array> pLonInfo, Handle<Array> pLatInfo, Handle<Array> lonInfo, Handle<Array> latInfo)
    {
        this->nth = 1;
        this->same = 2 * DBL_MIN;
        this->neighbor = 2 * DBL_MAX;
        this->loLim = -DBL_MAX;
        this->upLim = DBL_MAX;

        width = lonInfo->Length();
        height = latInfo->Length();
        pNum = pValueInfo->Length();

        pValues = (double *)malloc(sizeof(double) * pNum);
        pLons = (double *)malloc(sizeof(double) * pNum);
        pLats = (double *)malloc(sizeof(double) * pNum);
        lons = (double *)malloc(sizeof(double) * width);
        lats = (double *)malloc(sizeof(double) * height);
        for (int y = 0; y < height; y++)
        {
            lats[y] = Handle<Number>::Cast(latInfo->Get(y))->NumberValue();
        }
        for (int x = 0; x < width; x++)
        {
            lons[x] = Handle<Number>::Cast(lonInfo->Get(x))->NumberValue();
        }
        for (int p = 0; p < pNum; p++)
        {
            pValues[p] = Handle<Number>::Cast(pValueInfo->Get(p))->NumberValue();
            pLons[p] = Handle<Number>::Cast(pLonInfo->Get(p))->NumberValue();
            pLats[p] = Handle<Number>::Cast(pLatInfo->Get(p))->NumberValue();
        }

        return;
    }
    ~arguments()
    {
        free(pValues);
        free(pLons);
        free(pLats);
        free(lons);
        free(lats);
        return;
    }
};

struct asyncWork
{
    uv_work_t request;
    Persistent<Function> callback;
    arguments *args;
    double *result;
    ~asyncWork()
    {
        callback.Reset();
        delete args;
        free(result);
    }
};

double *idw(arguments &args)
{
    double *result = (double *)(malloc(sizeof(double) * args.height * args.width));
    for (int y = 0; y < args.height; y++)
    {
        double lat = args.lats[y];
        for (int x = 0; x < args.width; x++)
        {
            double lon = args.lons[x];
            double norm = 0;
            double tmpResult = 0;
            for (int p = 0; p < args.pNum; p++)
            {

                double xdist = lon - args.pLons[p];
                double ydist = lat - args.pLats[p];
                double dist = sqrt(xdist * xdist + ydist * ydist);
                if (dist > args.same)
                {
                    if (dist > args.neighbor || args.pValues[p] > args.upLim || args.pValues[p] < args.loLim)
                    {
                        continue;
                    }
                    dist = pow(dist, args.nth);
                    norm += 1 / dist;
                    tmpResult += args.pValues[p] / dist;
                }
                else
                {
                    tmpResult = args.pValues[p];
                    norm = 1;
                    break;
                }
            }
            result[y * args.width + x] = norm == 0 ? 0 : tmpResult / norm;
        }
    }
    return result;
}
void asyncWorker(uv_work_t *req)
{
    asyncWork *work = static_cast<asyncWork *>(req->data);
    work->result = idw(*(work->args));
    return;
}
Local<Array> transferResult(Isolate *isolate, double *src, arguments *args)
{
    Local<Array> dst = Array::New(isolate, args->height);
    for (int j = 0; j < args->height; j++)
    {
        Local<Array> row = Array::New(isolate, args->width);
        for (int i = 0; i < args->width; i++)
        {
            row->Set(i, Number::New(isolate, src[j * args->width + i]));
        }
        dst->Set(j, row);
    }
    return dst;
}
void asyncComplete(uv_work_t *req, int status)
{
    Isolate *isolate = Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);
    asyncWork *work = static_cast<asyncWork *>(req->data);

    Local<Array> result = transferResult(isolate, work->result, work->args);
    const int argc = 1;
    Handle<Value> argv[argc] = {result};
    Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    work->callback.Reset();

    delete work;
}
void async(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    arguments *arg = 0;
    switch (args.Length())
    {
    case 4:
        arg = new arguments(Handle<Array>::Cast(args[0]), Handle<Array>::Cast(args[1]), Handle<Array>::Cast(args[2]));
        break;
    case 5:
        arg = new arguments(Handle<Array>::Cast(args[0]), Handle<Array>::Cast(args[1]), Handle<Array>::Cast(args[2]), Handle<Object>::Cast(args[3]));
        break;
    case 6:
        arg = new arguments(Handle<Array>::Cast(args[0]), Handle<Array>::Cast(args[1]), Handle<Array>::Cast(args[2]), Handle<Array>::Cast(args[3]), Handle<Array>::Cast(args[4]));
        break;
    case 7:
        arg = new arguments(Handle<Array>::Cast(args[0]), Handle<Array>::Cast(args[1]), Handle<Array>::Cast(args[2]), Handle<Array>::Cast(args[3]), Handle<Array>::Cast(args[4]), Handle<Object>::Cast(args[5]));
        break;
    default:
        args.GetReturnValue().Set(Undefined(isolate));
        return;
    }
    Handle<Function> callback = Handle<Function>::Cast(args[args.Length() - 1]);
    asyncWork *work = new asyncWork();
    work->args = arg;
    work->request.data = work;
    work->callback.Reset(isolate, callback);
    uv_queue_work(uv_default_loop(), &work->request, asyncWorker, asyncComplete);
    args.GetReturnValue().Set(Undefined(isolate));
    return;
}
void sync(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    arguments *arg = 0;
    switch (args.Length())
    {
    case 3:
        arg = new arguments(Handle<Array>::Cast(args[0]), Handle<Array>::Cast(args[1]), Handle<Array>::Cast(args[2]));
        break;
    case 4:
        arg = new arguments(Handle<Array>::Cast(args[0]), Handle<Array>::Cast(args[1]), Handle<Array>::Cast(args[2]), Handle<Object>::Cast(args[3]));
        break;
    case 5:
        arg = new arguments(Handle<Array>::Cast(args[0]), Handle<Array>::Cast(args[1]), Handle<Array>::Cast(args[2]), Handle<Array>::Cast(args[3]), Handle<Array>::Cast(args[4]));
        break;
    case 6:
        arg = new arguments(Handle<Array>::Cast(args[0]), Handle<Array>::Cast(args[1]), Handle<Array>::Cast(args[2]), Handle<Array>::Cast(args[3]), Handle<Array>::Cast(args[4]), Handle<Object>::Cast(args[5]));
        break;
    default:
        args.GetReturnValue().Set(Undefined(isolate));
        return;
    }
    args.GetReturnValue().Set(transferResult(isolate, idw(*arg), arg));
    return;
}
void init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "sync", sync);
    NODE_SET_METHOD(exports, "async", async);
    // NODE_SET_METHOD(exports, "test", test);
}
NODE_MODULE(idw, init)
}