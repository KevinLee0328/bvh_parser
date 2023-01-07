#include<fstream>
#include"bvh_parser.h"

// a naive bvh parser

using std::ifstream;
std::string parent_name;

bool bvh_joint(std::istream& is, joint* root) {
	std::string token;
	is >> token;
	if (token == "}")
        return false;
    root->joint_type = token;
    if (root->joint_type == "End") {
        is >> token >> token >> token
            >> root->offset_x >> root->offset_y >> root->offset_z
            >> token;
        root->name = parent_name + "_End";
        return true;
    }
    is >> root->name >> token >> token
        >> root->offset_x >> root->offset_y >> root->offset_z;
	parent_name = root->name;
    int num;
    is >> token >> num;
    for (int i = 0; i < num; i++) {
        is >> token;
        root->channels.push_back(token);
    }
    while (1) {
        joint* children = new joint;
        if (!bvh_joint(is, children)) break;
        root->children.push_back(children);
    }
    return true;
}

bool bvh_META(std::istream& is, META* meta_data) {
    std::string token;
    is >> token >> meta_data->frame
        >> token >> token >> meta_data->frame_time;
    return true;
}

bool bvh_data(std::istream& is, joint* root) {
    vector<double> motion;
    double token;
    for (int i = 0; i < root->channels.size(); i++) {
        is >> token;
        motion.push_back(token);
    }
    root->motion.push_back(motion);
    if (root->joint_type != "End") 
        for (int i = 0; i < root->children.size(); i++)
            bvh_data(is, root->children[i]);
    return true;
}

int main(int argc, char** argv) {
    joint root;
    META meta_data;
    ifstream file(argv[1]);
    if (!file.is_open()) exit(1);
    std::string token;
    file >> token;
    bvh_joint(file, &root);
    file >> token;
    bvh_META(file, &meta_data);
    for (int i = 0; i < meta_data.frame; i++)
        bvh_data(file, &root);
    jsonify(root, meta_data);
    file.close();
    return 0;
}
