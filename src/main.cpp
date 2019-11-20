#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "omp.h"

#include "ipelib.h"
#include "ibex_IntervalVector.h"

#include "figure.h"

using namespace std;

int main(int argc, char *argv[]){
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
    fig.set_color_stroke("red");
    fig.set_color_type(ipegenerator::Figure::STROKE_AND_FILL);
    fig.set_opacity(30);
    fig.draw_circle(10, 1.0, 2.0);

    fig.draw_circle_radius_final(100.0, 1.0, 10.0);

    fig.set_dashed("dotted");
    fig.draw_sector(500.0, 1.0, 1.0, 1.0, 0.0, M_PI_2);

    fig.add_layer("test");
    fig.set_visible("test");

    fig.save_ipe("test.ipe");
    fig.save_pdf("test.pdf");
}

