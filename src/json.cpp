#include<iostream>
#include<fstream>
#include"bvh_parser.h"

using std::ofstream;
using std::endl;

void output_joint(std::ostream& os, joint* root, int tab) {
    string space(tab, '\t');
    os << space << "{" << endl
        << space << "\t\"type\": \"" << root->joint_type << "\"," << endl
        << space << "\t\"name\": \"" << root->name << "\"," << endl
        << space << "\t\"offset\": [" << root->offset_x << ", " << root->offset_y << ", " << root->offset_z << "]," << endl
        << space << "\t\"channels\": [";
	for (int i = 0; i < root->channels.size(); i++) {
		os << "\"" << root->channels[i] << "\"";
		if (i != root->channels.size() - 1) os << ", ";
	}
    os << "]," << endl
		<< space << "\t\"motion\": [" << endl;
    for (int i = 0; i < root->motion.size(); i++) {
        os << space << "\t\t[";
        for (int j = 0; j < root->channels.size(); j++) {
            os << root->motion[i][j];
            if (j != root->channels.size() - 1) os << ", ";
        }
		os << "]";
		if (i != root->motion.size() - 1) os << ",";
		os << endl;
    }
	os << space << "\t]," << endl
		<< space << "\t\"children\": [" << endl;
	for (int i = 0; i < root->children.size(); i++) {
		output_joint(os, root->children[i], tab + 2);
		if (i != root->children.size() - 1) os << "," << endl;
	}
    os << endl
        << space << "\t]" << endl
        << space << "}" << endl;
}

void jsonify(joint root, META meta_data) {
    ofstream outfile;
    outfile.open("output.json");
    
    // output the root and meta_data

    outfile << "{" << endl
        << "\t\"frame\": " << meta_data.frame << "," << endl
		<< "\t\"frame_time\": " << meta_data.frame_time << "," << endl
		<< "\t\"joint\":" << endl;
    output_joint(outfile, &root, 2);
    outfile << "}" << endl;
    outfile.close();
}