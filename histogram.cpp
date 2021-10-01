#include <list>                // std::list
#include <boost/format.hpp>    // only needed for printing
#include <functional>          // std::ref
#include <numeric>             // std::accumulate
#include "histogram.h"
#include "vars.h"

std::list<double> init_bins(int cell_no) {
    //initalise histogram bins db
    std::vector<int> b(cell_no + 1);
    std::iota(b.begin(), b.end(), 0);

    
    double cell_L = cell_length;
    std::vector<double> db(b.begin(), b.end());
    std::transform(db.begin(), db.end(), db.begin(), [&cell_L](auto& c) {return c * cell_L;});

    std::list<double> bins(db.begin(), db.end());

    return bins;
}

std::vector<double> rho_dist(std::vector<double> x) {

    std::list<double> bins_list = init_bins(std::round(L / cell_length));
    //auto bins = axis::regular<double, use_default, axis::option::none_t>{ axis::step(cell_length), 0, L };

    auto bins = axis::variable<double, use_default, axis::option::none_t>(bins_list);

    auto h_rho = make_histogram(bins); //initialise histogram of length L with bins of width cell_length

    std::for_each(x.begin(), x.end(), std::ref(h_rho)); //populate histogram with x_evolved

    std::vector<double> vec_h_rho(h_rho.axis().size());
    
    for (unsigned i = 0; i < vec_h_rho.size(); i++) {        
        vec_h_rho[i] = h_rho.at(i); //convert hist to vector because C++ can't deal with autos :(
    }
    return vec_h_rho;
}

/*
* Create averaged cell velocity distribution using density histogram
*/

std::vector<double> cell_velocity_dist(std::vector<double> v, std::vector<double> x) {
    
    std::vector<double> vel_hist;
    unsigned cum_bincount = 0;

    // calculate all the mean cell velocities bar the last cell
    for (unsigned i = 0; i != x.size(); i++) {
        unsigned n_i = x.at(i);

        if (n_i == 0) {
            vel_hist.push_back(0);
        }

        else {
            double splice_sum = std::accumulate(v.begin() + cum_bincount, v.begin() + (cum_bincount + n_i), 0.0);
            double cell_vbar = splice_sum / n_i;
            vel_hist.push_back(cell_vbar);
            cum_bincount += n_i;
        }

    }
    return vel_hist;

}
/*
* Create histograms {h_rho,h_j} for a sample run
*/
std::vector<std::vector<double>> rho_j_dist(std::vector<double> x, std::vector<double> v, std::list<double> bins_list) {

    //std::list<double> bins_list = init_bins(std::round(L / cell_length));
    //auto bins = axis::regular<double, use_default, axis::option::none_t>{ axis::step(cell_length), 0, L };
    auto bins = axis::variable<>(bins_list);

    auto h_rho = make_histogram(bins); //initialise histogram of length L with bins of width cell_length

    std::for_each(x.begin(), x.end(), std::ref(h_rho)); //populate histogram with x_evolved

    std::vector<double> vec_h_rho(h_rho.axis().size());

    for (unsigned i = 0; i != vec_h_rho.size(); i++) {
        vec_h_rho[i] = h_rho.at(i); //convert hist to vector because C++ can't deal with autos    }
    }

    std::vector<double> h_v = cell_velocity_dist(v, vec_h_rho);
    
    std::vector<double> h_j(h_v.size());

    for (unsigned i = 0; i < h_v.size(); i++) {
        h_j[i] = h_v[i] *h_rho.at(i);
    }

    return { vec_h_rho,h_v };
}


/*
* compute averaged_rho_dist with a matrix of rod_positions (dim. no_samples * no_rods) as input
*/
auto averaged_rho_dist(std::vector<std::vector<double>> x_matrix) {

    auto bins = axis::regular<double, use_default, axis::option::none_t>{ axis::step(cell_length), 0, L };

    auto h_rho = make_histogram(bins); //initialise histogram of length L with bins of width cell_length

    for (unsigned i = 0; i < x_matrix.size(); i++) {
        for (unsigned i = 0; i < x_matrix[i].size(); i++) {
            std::for_each(x_matrix[i].begin(), x_matrix[i].end(), std::ref(h_rho));
        }
    }

    return h_rho / x_matrix.size();
}


