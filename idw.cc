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
    if (args.Length() == 3)
    {
        Local<Array> points = Local<Array>::Cast(args[0]);
        Local<Array> lons = Local<Array>::Cast(args[1]);
        Local<Array> lats = Local<Array>::Cast(args[2]);

        int width = lons->Length();
        int height = lats->Length();
        int pointNum = points->Length();
        Local<Array> result = Array::New(isolate, height);
        for (int y = 0; y < height; y++)
        {
            Local<Number> lat = Local<Number>::Cast(lats->Get(y));
            Local<Array> column = Array::New(isolate, width);
            for (int x = 0; x < width; x++)
            {
                Local<Number> lon = Local<Number>::Cast(lons->Get(x));
                double norm = 0;
                double tmpResult = 0;

                for (int p = 0; p < pointNum; p++)
                {
                    Local<Object> point = Local<Object>::Cast(points->Get(p));
                    Local<Number> pLon = Local<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "longitude")));
                    Local<Number> pLat = Local<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "latitude")));
                    Local<Number> pValue = Local<Number>::Cast(point->Get(String::NewFromUtf8(isolate, "value")));
                    double xdist = (lon->NumberValue() - pLon->NumberValue());
                    double ydist = (lat->NumberValue() - pLat->NumberValue());
                    double dist = sqrt(xdist * xdist + ydist * ydist);
                    if (dist)
                    {
                        norm += 1 / dist;
                        tmpResult += pValue->NumberValue() / dist;
                    }
                    else
                    {
                        tmpResult = pValue->NumberValue();
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
    if (args.Length() == 5)
    {
        Local<Array> pValues = Local<Array>::Cast(args[0]);
        Local<Array> pLons = Local<Array>::Cast(args[1]);
        Local<Array> pLats = Local<Array>::Cast(args[2]);
        Local<Array> lons = Local<Array>::Cast(args[3]);
        Local<Array> lats = Local<Array>::Cast(args[4]);

        if (pValues->Length() != pLons->Length() || pValues->Length() != pLats->Length())
        {
            args.GetReturnValue().SetUndefined();
            return;
        }

        int width = lons->Length();
        int height = lats->Length();
        int pointNum = pValues->Length();
        Local<Array> result = Array::New(isolate, height);
        for (int y = 0; y < height; y++)
        {
            Local<Number> lat = Local<Number>::Cast(lats->Get(y));
            Local<Array> column = Array::New(isolate, width);
            for (int x = 0; x < width; x++)
            {
                Local<Number> lon = Local<Number>::Cast(lons->Get(x));
                double norm = 0;
                double tmpResult = 0;

                for (int p = 0; p < pointNum; p++)
                {
                    Local<Number> pLon = Local<Number>::Cast(pLons->Get(p));
                    Local<Number> pLat = Local<Number>::Cast(pLats->Get(p));
                    Local<Number> pValue = Local<Number>::Cast(pValues->Get(p));
                    double xdist = (lon->NumberValue() - pLon->NumberValue());
                    double ydist = (lat->NumberValue() - pLat->NumberValue());
                    double dist = sqrt(xdist * xdist + ydist * ydist);
                    if (dist)
                    {
                        norm += 1 / dist;
                        tmpResult += pValue->NumberValue() / dist;
                    }
                    else
                    {
                        tmpResult = pValue->NumberValue();
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
}
void init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "idw", idw);
}
NODE_MODULE(idw, init)
}
