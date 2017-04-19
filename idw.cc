#include <node.h>
#include <v8.h>
#include <math.h>
using namespace v8;
using namespace std;
namespace idw
{
void idw(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    Isolate *isolate = Isolate::GetCurrent();
    double *pValues = (double *)0;
    double *pLons = (double *)0;
    double *pLats = (double *)0;
    double *lons = (double *)0;
    double *lats = (double *)0;
    int width = 0;
    int height = 0;
    int pNum = 0;

    if (args.Length() == 3)
    {
        Local<Array> pointInfo = Local<Array>::Cast(args[0]);
        Local<Array> lonInfo = Local<Array>::Cast(args[1]);
        Local<Array> latInfo = Local<Array>::Cast(args[2]);

        width = lonInfo->Length();
        height = latInfo->Length();
        pNum = pointInfo->Length();
        pValues = (double *)malloc(sizeof(double) * pNum);
        pLons = (double *)malloc(sizeof(double) * pNum);
        pLats = (double *)malloc(sizeof(double) * pNum);
        lons = (double *)malloc(sizeof(double) * width);
        lats = (double *)malloc(sizeof(double) * height);
        for (int y = 0; y < height; y++)
        {
            lats[y] = Local<Number>::Cast(latInfo->Get(y))->NumberValue();
        }
        for (int x = 0; x < width; x++)
        {
            lons[x] = Local<Number>::Cast(latInfo->Get(x))->NumberValue();
        }
        for (int p = 0; p < pNum; p++)
        {
            Local<Object> point = Local<Object>::Cast(pointInfo->Get(p));
            pValues[p] = Local<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "value")))->NumberValue();
            pLons[p] = Local<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "longitude")))->NumberValue();
            pLats[p] = Local<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "latitude")))->NumberValue();
        }
    }
    else
    {
        if (args.Length() == 5)
        {
            Local<Array> pValueInfo = Local<Array>::Cast(args[0]);
            Local<Array> pLonInfo = Local<Array>::Cast(args[1]);
            Local<Array> pLatInfo = Local<Array>::Cast(args[2]);
            Local<Array> lonInfo = Local<Array>::Cast(args[3]);
            Local<Array> latInfo = Local<Array>::Cast(args[4]);

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
                lats[y] = Local<Number>::Cast(latInfo->Get(y))->NumberValue();
            }
            for (int x = 0; x < width; x++)
            {
                lons[x] = Local<Number>::Cast(latInfo->Get(x))->NumberValue();
            }
            for (int p = 0; p < pNum; p++)
            {
                pValues[p] = Local<Number>::Cast(pValueInfo->Get(p))->NumberValue();
                pLons[p] = Local<Number>::Cast(pLonInfo->Get(p))->NumberValue();
                pLats[p] = Local<Number>::Cast(pLatInfo->Get(p))->NumberValue();
            }
        }
    }
    Local<Array> result = Array::New(isolate, height);
    for (int y = 0; y < height; y++)
    {
        double lat = lats[y];
        Local<Array> column = Array::New(isolate, width);

        for (int x = 0; x < width; x++)
        {
            double lon = lons[x];
            double norm = 0;
            double tmpResult = 0;
            for (int p = 0; p < pNum; p++)
            {
                double xdist = lon - pLons[p];
                double ydist = lat - pLats[p];
                double dist = sqrt(xdist * xdist + ydist * ydist);
                if (dist != 0)
                {
                    norm += 1 / dist;
                    tmpResult += pValues[p] / dist;
                }
                else
                {
                    tmpResult = pValues[p];
                    norm = 1;
                    break;
                }
            }
            column->Set(x, Number::New(isolate, tmpResult / norm));
        }
        result->Set(y, column);
    }
    args.GetReturnValue().Set(result);
    return;
}
void init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "idw", idw);
}
NODE_MODULE(idw, init)
}