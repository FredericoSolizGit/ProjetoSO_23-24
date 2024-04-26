#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "requisito_1.h"
#include "monteCarlo.h"
#include "requisito_2.h"
#include "requisito_3.h"
#include "requisito_4.h"

#define NUM_POINTS 1000000
#define fmax
#define fmin


Point generateRandomPoint() {
    Point p;
    p.x = ((double)rand() / RAND_MAX) * 2 - 1; // Intervalo de -1 a 1
    p.y = ((double)rand() / RAND_MAX) * 2 - 1; // Intervalo de -1 a 1
    return p;
}

int orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}

bool onSegment(Point p, Point q, Point r) {
    if (q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&
        q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y))
        return true;

    return false;
}

bool doIntersect(Point p1, Point q1, Point p2, Point q2) {

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

bool isInsidePolygon(Point polygon[], int n, Point p) {

    if (n < 3) return false;

    Point extreme = {2.5, p.y};

    int count = 0, i = 0;
    do {
        int next = (i+1)%n;

        if (doIntersect(polygon[i], polygon[next], p, extreme)) {
            if (orientation(polygon[i], p, polygon[next]) == 0)
                return onSegment(polygon[i], p, polygon[next]);
            count++;
        }
        i = next;
    } while (i != 0);

    return count&1;
}




int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc != 4) {
        printf("Usage: %s <filename> <num_processes> <num_points>\n", argv[0]);
        return 1;
    }


    Point polygon[] = {{0.981644, 0.000000}, {0.985264, 0.012092}, {0.977092, 0.023986}, {0.968619, 0.035676}, {0.967894, 0.047550}, {0.981037, 0.060271}, {0.993850, 0.073311}, {0.986893, 0.084986}, {0.992182, 0.097721}, {0.971741, 0.107764}, {0.991878, 0.122336}, {0.973373, 0.132200}, {0.984798, 0.146081}, {0.969935, 0.156064}, {0.970193, 0.168345}, {0.978908, 0.182258}, {0.961599, 0.191274}, {0.966849, 0.204684}, {0.975363, 0.219025}, {0.951291, 0.225917}, {0.962839, 0.241179}, {0.948803, 0.250076}, {0.926619, 0.256430}, {0.943624, 0.273646}, {0.933652, 0.283220}, {0.945929, 0.299669}, {0.940088, 0.310563}, {0.922749, 0.317447}, {0.916394, 0.327891}, {0.901794, 0.335206}, {0.922258, 0.355754}, {0.902728, 0.361008}, {0.893899, 0.370265}, {0.906395, 0.388540}, {0.878058, 0.389216}, {0.887223, 0.406378}, {0.886910, 0.419477}, {0.881943, 0.430450}, {0.883451, 0.444692}, {0.880532, 0.456851}, {0.876132, 0.468302}, {0.841111, 0.462943}, {0.858362, 0.486256}, {0.839587, 0.489326}, {0.846466, 0.507352}, {0.832153, 0.512758}, {0.841418, 0.532822}, {0.824101, 0.536137}, {0.819842, 0.547801}, {0.823333, 0.564870}, {0.809497, 0.570112}, {0.802943, 0.580366}, {0.791380, 0.586928}, {0.793258, 0.603549}, {0.777387, 0.606678}, {0.759651, 0.607983}, {0.764087, 0.627070}, {0.763838, 0.642713}, {0.742905, 0.640835}, {0.732400, 0.647617}, {0.714518, 0.647601}, {0.709675, 0.659254}, {0.711257, 0.677173}, {0.687601, 0.670929}, {0.691960, 0.691960}, {0.678709, 0.695574}, {0.681824, 0.716143}, {0.665184, 0.716059}, {0.655043, 0.722728}, {0.653530, 0.739088}, {0.644068, 0.746654}, {0.629771, 0.748456}, {0.610899, 0.744383}, {0.600124, 0.749831}, {0.610674, 0.782506}, {0.604671, 0.794732}, {0.578421, 0.779911}, {0.569766, 0.788277}, {0.556767, 0.790549}, {0.564486, 0.822773}, {0.551395, 0.825221}, {0.538358, 0.827516}, {0.519107, 0.819759}, {0.508823, 0.825767}, {0.510776, 0.852179}, {0.498167, 0.854756}, {0.482421, 0.851592}, {0.477651, 0.867834}, {0.453593, 0.848613}, {0.446799, 0.861157}, {0.448149, 0.890319}, {0.425235, 0.871258}, {0.421767, 0.891751}, {0.414644, 0.905270}, {0.393017, 0.886632}, {0.383743, 0.895205}, {0.368021, 0.888481}, {0.358992, 0.897687}, {0.351031, 0.910014}, {0.343220, 0.923354}, {0.333525, 0.932140}, {0.318656, 0.926265}, {0.307224, 0.929981}, {0.299514, 0.945440}, {0.282475, 0.931196}, {0.271950, 0.937776}, {0.262274, 0.947735}, {0.248994, 0.944700}, {0.235929, 0.941881}, {0.228607, 0.962618}, {0.213728, 0.951773}, {0.203097, 0.959357}, {0.189567, 0.953017}, {0.175820, 0.944329}, {0.168004, 0.968228}, {0.156572, 0.973091}, {0.142066, 0.957728}, {0.132735, 0.977312}, {0.119849, 0.971710}, {0.109352, 0.986057}, {0.096503, 0.979814}, {0.083713, 0.972105}, {0.072184, 0.978571}, {0.060773, 0.989197}, {0.048155, 0.980218}, {0.036045, 0.978624}, {0.023988, 0.977144}, {0.011945, 0.973297}, {0.000000, 0.962446}, {-0.011811, 0.962409}, {-0.024371, 0.992781}, {-0.036394, 0.988106}, {-0.047566, 0.968231}, {-0.060652, 0.987236}, {-0.070662, 0.957943}, {-0.082619, 0.959402}, {-0.095058, 0.965144}, {-0.106702, 0.962165}, {-0.117749, 0.954681}, {-0.133865, 0.985631}, {-0.146013, 0.984340}, {-0.155563, 0.966822}, {-0.170057, 0.980062}, {-0.177493, 0.953312}, {-0.187968, 0.944977}, {-0.204228, 0.964699}, {-0.213961, 0.952814}, {-0.222321, 0.936150}, {-0.240033, 0.958266}, {-0.246093, 0.933692}, {-0.264317, 0.955118}, {-0.268635, 0.926345}, {-0.286128, 0.943238}, {-0.293790, 0.927371}, {-0.306187, 0.926843}, {-0.313769, 0.912059}, {-0.333304, 0.931523}, {-0.341634, 0.919087}, {-0.355754, 0.922257}, {-0.356841, 0.892307}, {-0.376231, 0.908302}, {-0.382221, 0.891655}, {-0.402187, 0.907318}, {-0.410434, 0.896078}, {-0.420531, 0.889137}, {-0.422340, 0.865326}, {-0.442385, 0.878868}, {-0.451366, 0.869961}, {-0.468925, 0.877297}, {-0.470639, 0.855095}, {-0.479326, 0.846129}, {-0.483822, 0.830144}, {-0.508747, 0.848793}, {-0.516257, 0.837831}, {-0.515570, 0.814174}, {-0.543427, 0.835308}, {-0.534508, 0.799947}, {-0.553751, 0.807127}, {-0.556026, 0.789497}, {-0.566291, 0.783470}, {-0.584619, 0.788268}, {-0.605339, 0.795611}, {-0.596610, 0.764486}, {-0.624857, 0.780735}, {-0.609707, 0.742930}, {-0.624508, 0.742201}, {-0.643755, 0.746291}, {-0.646326, 0.730941}, {-0.648346, 0.715340}, {-0.679709, 0.731694}, {-0.664657, 0.698111}, {-0.681328, 0.698259}, {-0.699964, 0.699964}, {-0.697969, 0.681045}, {-0.705525, 0.671716}, {-0.715515, 0.664679}, {-0.740204, 0.670882}, {-0.739063, 0.653508}, {-0.738965, 0.637435}, {-0.750612, 0.631584}, {-0.747234, 0.613239}, {-0.750654, 0.600783}, {-0.766412, 0.598114}, {-0.782920, 0.595684}, {-0.796305, 0.590580}, {-0.781734, 0.565037}, {-0.787647, 0.554724}, {-0.805720, 0.552786}, {-0.830524, 0.554938}, {-0.831850, 0.541178}, {-0.820018, 0.519271}, {-0.838058, 0.516396}, {-0.828893, 0.496820}, {-0.847562, 0.493974}, {-0.868713, 0.492120}, {-0.841273, 0.463032}, {-0.869347, 0.464676}, {-0.874884, 0.453920}, {-0.876290, 0.441087}, {-0.896763, 0.437683}, {-0.894092, 0.422874}, {-0.893986, 0.409476}, {-0.888043, 0.393643}, {-0.899366, 0.385527}, {-0.904865, 0.374807}, {-0.904307, 0.361640}, {-0.910284, 0.351135}, {-0.925317, 0.343950}, {-0.928284, 0.332145}, {-0.909628, 0.312933}, {-0.938568, 0.310061}, {-0.947159, 0.300059}, {-0.922146, 0.279730}, {-0.922277, 0.267455}, {-0.963235, 0.266563}, {-0.950025, 0.250398}, {-0.943380, 0.236304}, {-0.960539, 0.228113}, {-0.960248, 0.215631}, {-0.956773, 0.202550}, {-0.971340, 0.193212}, {-0.981548, 0.182750}, {-0.963687, 0.167216}, {-0.979679, 0.157632}, {-0.970952, 0.144027}, {-0.977871, 0.132811}, {-0.990181, 0.122127}, {-0.987334, 0.109493}, {-0.968115, 0.095351}, {-0.994912, 0.085677}, {-0.984035, 0.072587}, {-0.968071, 0.059475}, {-0.966507, 0.047481}, {-0.994370, 0.036625}, {-0.970681, 0.023829}, {-0.972811, 0.011939}, {-0.962404, 0.000000}, {-0.974254, -0.011956}, {-0.964773, -0.023684}, {-0.965505, -0.035562}, {-0.971494, -0.047726}, {-0.962071, -0.059106}, {-0.957900, -0.070659}, {-0.981415, -0.084514}, {-0.994387, -0.097939}, {-0.968116, -0.107362}, {-0.988248, -0.121889}, {-0.974351, -0.132333}, {-0.984573, -0.146048}, {-0.952045, -0.153185}, {-0.975591, -0.169281}, {-0.979113, -0.182296}, {-0.978939, -0.194723}, {-0.948155, -0.200726}, {-0.936724, -0.210348}, {-0.941874, -0.223680}, {-0.950021, -0.237968}, {-0.930818, -0.245335}, {-0.962153, -0.266264}, {-0.950385, -0.275607}, {-0.934277, -0.283410}, {-0.949287, -0.300733}, {-0.945288, -0.312281}, {-0.933089, -0.321004}, {-0.926612, -0.331547}, {-0.935600, -0.347772}, {-0.924633, -0.356670}, {-0.924945, -0.369893}, {-0.896919, -0.371516}, {-0.898433, -0.385127}, {-0.891533, -0.395189}, {-0.892172, -0.408645}, {-0.884708, -0.418436}, {-0.871602, -0.425403}, {-0.881256, -0.443587}, {-0.865027, -0.448806}, {-0.873625, -0.466962}, {-0.857445, -0.471933}, {-0.844053, -0.478150}, {-0.863916, -0.503505}, {-0.845812, -0.506960}, {-0.835772, -0.514988}, {-0.825778, -0.522918}, {-0.826021, -0.537385}, {-0.828227, -0.553404}, {-0.812462, -0.557411}, {-0.801946, -0.564794}, {-0.797118, -0.576157}, {-0.795896, -0.590277}, {-0.777672, -0.591690}, {-0.786521, -0.613807}, {-0.763257, -0.610869}, {-0.758116, -0.622170}, {-0.757069, -0.637018}, {-0.754041, -0.650440}, {-0.722730, -0.639066}, {-0.734915, -0.666088}, {-0.719952, -0.668801}, {-0.708317, -0.674374}, {-0.711418, -0.694168}, {-0.698307, -0.698307}, {-0.684554, -0.701566}, {-0.669808, -0.703522}, {-0.661322, -0.711901}, {-0.650974, -0.718239}, {-0.652562, -0.737992}, {-0.652218, -0.756102}, {-0.635383, -0.755126}, {-0.625377, -0.762024}, {-0.609983, -0.762150}, {-0.614383, -0.787259}, {-0.602113, -0.791371}, {-0.594310, -0.801334}, {-0.584339, -0.808438}, {-0.561614, -0.797430}, {-0.556722, -0.811457}, {-0.535168, -0.800936}, {-0.529362, -0.813688}, {-0.534433, -0.843963}, {-0.524085, -0.850536}, {-0.510691, -0.852037}, {-0.497959, -0.854400}, {-0.492573, -0.869513}, {-0.478828, -0.869973}, {-0.471321, -0.881780}, {-0.443824, -0.855424}, {-0.442477, -0.879051}, {-0.438032, -0.897478}, {-0.414679, -0.876765}, {-0.411391, -0.898168}, {-0.391941, -0.884204}, {-0.383610, -0.894895}, {-0.374860, -0.904993}, {-0.361929, -0.905031}, {-0.355976, -0.922834}, {-0.336454, -0.905151}, {-0.326776, -0.913278}, {-0.312704, -0.908964}, {-0.311445, -0.942757}, {-0.295823, -0.933790}, {-0.283447, -0.934399}, {-0.271867, -0.937489}, {-0.263982, -0.953910}, {-0.251448, -0.954011}, {-0.237810, -0.949390}, {-0.225268, -0.948560}, {-0.213095, -0.948954}, {-0.200545, -0.947298}, {-0.190795, -0.959193}, {-0.178688, -0.959732}, {-0.168787, -0.972742}, {-0.153809, -0.955923}, {-0.140883, -0.949756}, {-0.134325, -0.989020}, {-0.119532, -0.969142}, {-0.106811, -0.963143}, {-0.096510, -0.979881}, {-0.084659, -0.983098}, {-0.072676, -0.985245}, {-0.059549, -0.969284}, {-0.048224, -0.981614}, {-0.036362, -0.987238}, {-0.023674, -0.964357}, {-0.012206, -0.994561}, {-0.000000, -0.979368}, {0.011957, -0.974293}, {0.023566, -0.959966}, {0.035765, -0.971017}, {0.048149, -0.980086}, {0.059339, -0.965858}, {0.070973, -0.962159}, {0.082591, -0.959076}, {0.095524, -0.969870}, {0.109631, -0.988574}, {0.119472, -0.968657}, {0.132973, -0.979067}, {0.144111, -0.971518}, {0.154846, -0.962369}, {0.165440, -0.953452}, {0.182441, -0.979889}, {0.189700, -0.953686}, {0.203280, -0.960221}, {0.212973, -0.948413}, {0.222905, -0.938609}, {0.240760, -0.961166}, {0.254816, -0.966787}, {0.257207, -0.929426}, {0.275012, -0.948336}, {0.280133, -0.923476}, {0.294306, -0.929001}, {0.306448, -0.927630}, {0.321276, -0.933882}, {0.333627, -0.932426}, {0.343066, -0.922940}, {0.354489, -0.918978}, {0.357673, -0.894388}, {0.373808, -0.902452}, {0.386853, -0.902460}, {0.402792, -0.908684}, {0.410874, -0.897040}, {0.421444, -0.891068}, {0.432561, -0.886269}, {0.435787, -0.865760}, {0.444183, -0.856116}, {0.470065, -0.879430}, {0.474456, -0.862030}, {0.482739, -0.852154}, {0.490166, -0.841029}, {0.499959, -0.834132}, {0.520925, -0.845408}, {0.521336, -0.823280}, {0.540364, -0.830600}, {0.538661, -0.806162}, {0.564704, -0.823091}, {0.573617, -0.814473}, {0.581829, -0.804967}, {0.590788, -0.796585}, {0.587135, -0.771685}, {0.594225, -0.761429}, {0.607416, -0.758942}, {0.620581, -0.756180}, {0.636140, -0.756027}, {0.643317, -0.745783}, {0.661208, -0.747770}, {0.649101, -0.716172}, {0.675831, -0.727519}, {0.666184, -0.699714}, {0.694427, -0.711683}, {0.694708, -0.694708}, {0.708968, -0.691778}, {0.720196, -0.685684}, {0.730302, -0.678416}, {0.731349, -0.662856}, {0.746814, -0.660363}, {0.731221, -0.630756}, {0.735640, -0.618986}, {0.745499, -0.611815}, {0.762369, -0.610159}, {0.780775, -0.609323}, {0.772657, -0.587875}, {0.791747, -0.587199}, {0.808421, -0.584326}, {0.799669, -0.563191}, {0.797249, -0.546974}, {0.810656, -0.541663}, {0.814673, -0.530003}, {0.825604, -0.522808}, {0.844577, -0.520413}, {0.853688, -0.511681}, {0.861616, -0.502165}, {0.866504, -0.490868}, {0.863076, -0.475032}, {0.857505, -0.458346}, {0.856827, -0.444552}, {0.879027, -0.442465}, {0.883014, -0.430973}, {0.903028, -0.427101}, {0.894258, -0.409600}, {0.906833, -0.401971}, {0.882678, -0.378373}, {0.896929, -0.371520}, {0.907865, -0.363063}, {0.900834, -0.347490}, {0.918917, -0.341571}, {0.905116, -0.323856}, {0.923381, -0.317664}, {0.920775, -0.304183}, {0.919280, -0.291227}, {0.924011, -0.280296}, {0.929458, -0.269538}, {0.960162, -0.265712}, {0.965652, -0.254516}, {0.941045, -0.235719}, {0.945232, -0.224478}, {0.965301, -0.216766}, {0.953878, -0.201937}, {0.976741, -0.194286}, {0.959934, -0.178726}, {0.972787, -0.168795}, {0.961442, -0.154697}, {0.987398, -0.146467}, {0.952244, -0.129330}, {0.962806, -0.118751}, {0.983360, -0.109053}, {0.986287, -0.097141}, {0.976904, -0.084126}, {0.976015, -0.071995}, {0.961269, -0.059057}, {0.990836, -0.048677}, {0.963411, -0.035485}, {0.974140, -0.023914}, {0.987709, -0.012122}};
    int n = sizeof(polygon)/sizeof(polygon[0]);

    char *filename = argv[1];
    int num_processes = atoi(argv[2]);
    int num_points_per_process = atoi(argv[3]);

    //requisito_1(filename, num_processes, num_points_per_process);
    //requisito_2(filename, num_processes, num_points_per_process, polygon, 512);
    //requisito_3(filename, num_processes, num_points_per_process, polygon, 512);
    requisito_4(filename, num_processes, num_points_per_process, polygon, 512);




    return 0;
}


/*  FILE *file = fopen ("polygon.txt", "w");
    for (int i = 0; i < n; ++i) {
        fprintf(file, "%f %f\n", polygon[i].x, polygon[i].y);


    }
    fclose(file);
*/

