#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "omp.h"

#include "ipelib.h"
#include "ibex_IntervalVector.h"
#include "codac.h"

#include "figure.h"

using namespace std;


void test1()
{
    ibex::IntervalVector frame_data(2);
    frame_data[0] = ibex::Interval(0.0, 1000);
    frame_data[1] = ibex::Interval(-0.3, 2.0);



    ipegenerator::Figure fig(frame_data, 120, 50);

    fig.set_graduation_parameters(0.0, 100.0, 0.0, 0.5);
    fig.draw_axis("x_1", "x_2");

    ibex::IntervalVector box(2);
    box[0] = ibex::Interval(100.0, 300.0);
    box[1] = ibex::Interval(1.0, 1.5);
    fig.draw_box(box);


    fig.draw_text("test", 100, 1.0);

    vector<double> x, y;
    for(double t=0.0; t<1000.0; ++t)
    {
        x.push_back(t);
        y.push_back(sin(t/100.));
    }
    fig.draw_curve(x, y);

    fig.set_color_fill("black");
    fig.set_color_stroke(0,1000,0);
    fig.set_color_type(ipegenerator::STROKE_AND_FILL);
    fig.set_opacity(30);
    fig.draw_circle(10, 1.0, 0.5);

    fig.draw_circle_radius_final(100.0, 0.5, 10.0);

    fig.set_dashed("dotted");
    fig.draw_sector(500.0, 1.0, 100.0, 1.0, 0.0, M_PI_2);

    fig.add_layer("test");
    fig.set_visible("test");

    fig.save_ipe("test1.ipe");
    fig.save_pdf("test1.pdf");
}

void test2()
{
    ibex::IntervalVector frame_data(2);
    frame_data[0] = ibex::Interval(0.0, 10.0);
    frame_data[1] = ibex::Interval(0.0, 10.0);

    ipegenerator::Figure fig(frame_data, 200, 200);

    fig.draw_float(0.0, 0, 0.0, 0.0);
    fig.draw_float(1.0, 0, 1.0, 0.0);
    fig.draw_float(2.0, 0, 0.0, 1.0, ipegenerator::FLOAT_PISTON_DOWN);
    fig.draw_float(3.0, 0, 1.0, 1.0);
    fig.draw_float(5.0, 0, 1.0, 1.0, ipegenerator::FLOAT_PISTON_UP, 0.2);

    fig.draw_symbol(2.0, 2.0, "disk(sx)", 5.0);

    fig.save_ipe("test2.ipe");
    fig.save_pdf("test2.pdf");
}

void test3(){
    ibex::IntervalVector frame_data(2);
    frame_data[0] = ibex::Interval(0.0, 1000);
    frame_data[1] = ibex::Interval(-10.0, 2.0);

    ipegenerator::Figure fig(frame_data, 120, 50);

    fig.set_inverted_y();
    fig.set_number_digits_axis_x(0);
    fig.set_number_digits_axis_y(1);
    fig.set_graduation_parameters(0.0, 100.0, -10.0, 2.0);
    fig.draw_axis("x_1", "x_2");

    ibex::IntervalVector box(2);
    box[0] = ibex::Interval(100.0, 150.0);
    box[1] = ibex::Interval(1.0, 1.5);
    fig.draw_box(box,"red","black");

    ibex::IntervalVector box2(2);
    box2[0] = ibex::Interval(200, 250.0);
    box2[1] = ibex::Interval(1.0, 1.5);
    fig.draw_box(box2,"","blue");

    ibex::IntervalVector box3(2);
    box3[0] = ibex::Interval(300.0, 350.0);
    box3[1] = ibex::Interval(1.0, 1.5);
    fig.draw_box(box3,"green","");

    fig.save_ipe("test3.ipe");
    fig.save_pdf("test3.pdf");
}


void test4(){
    ibex::IntervalVector frame_data(2);
    frame_data[0] = ibex::Interval(0.0, 10);
    frame_data[1] = ibex::Interval(-10.0, 2.0);

    ipegenerator::Figure fig(frame_data, 120, 50);

    fig.set_number_digits_axis_x(0);
    fig.set_number_digits_axis_y(1);
    fig.set_graduation_parameters(0.0, 1.0, -10.0, 1.0);
    fig.draw_axis("x_1", "x_2");

    ibex::Interval domain(0,10);
    codac::Tube tube_sin(domain,0.1,codac::TFunction("sin(t)"));
    codac::Tube tube_cos(domain,0.1,codac::TFunction("cos(t)"));
    tube_sin.inflate(0.5);
    tube_cos.inflate(0.5);

    codac::TubeVector tubeVector(domain,0.1,codac::TFunction("(cos(t);sin(t))"));
    codac::TubeVector tubeVectorBig(tubeVector);
    tubeVectorBig.inflate(0.5);



    codac::ColorMap colorMap(codac::InterpolMode::RGB);
    codac::rgb red= codac::make_rgb((float)1.,(float)0.,(float)0.);
    codac::rgb green= codac::make_rgb((float)0.,(float)1.,(float)0.);
    colorMap.add_color_point(red,0);
    colorMap.add_color_point(green,1);
    fig.draw_tube(&tube_sin,&colorMap);


    fig.set_color_fill("red");
    fig.set_color_stroke(0,1000,0);
    fig.set_dashed("dotted");
    fig.set_color_type(ipegenerator::STROKE_AND_FILL);
    fig.set_opacity(30);
    fig.draw_tube(&tube_cos);



    fig.save_ipe("test4.ipe");
    fig.save_pdf("test4.pdf");
}

void test5()
{
    ibex::IntervalVector frame_data(2);
    frame_data[0] = ibex::Interval(-2., 2.);
    frame_data[1] = ibex::Interval(-2., 2.);
    ipegenerator::Figure fig(frame_data, 120, 50);

    fig.set_number_digits_axis_x(0);
    fig.set_number_digits_axis_y(1);
    fig.set_graduation_parameters(-2, 0.5, -2., 0.5);
    fig.draw_axis("x_1", "x_2");

    ibex::Interval domain(0,6);
    codac::TubeVector tubeVector(domain,0.1,codac::TFunction("(cos(t);sin(t))"));
    codac::TubeVector tubeVectorBig(tubeVector);
    tubeVectorBig.inflate(0.5);



    fig.set_color_fill("red");
    fig.set_color_stroke(0,1000,0);
    fig.set_dashed("dotted");
    fig.set_color_type(ipegenerator::STROKE_AND_FILL);
    fig.set_opacity(30);
    fig.draw_tubeVector(&tubeVectorBig,0,1);

    fig.set_opacity(100);

    codac::ColorMap colorMap(codac::InterpolMode::RGB);
    codac::rgb red= codac::make_rgb((float)1.,(float)0.,(float)0.);
    codac::rgb green= codac::make_rgb((float)0.,(float)1.,(float)0.);
    colorMap.add_color_point(red,0);
    colorMap.add_color_point(green,1);

    fig.draw_tubeVector(&tubeVector,0,1,&colorMap);



    fig.save_ipe("test5.ipe");
    fig.save_pdf("test5.pdf");
}


int main(int argc, char *argv[])
{
    test1();
    test2();
    test3();
    test4();
    test5();
}

