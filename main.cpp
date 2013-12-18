#include "vec.h"
#include "Forest.h"
#include <Eigen/Dense>

int main(int argc, char** argv)
{
	Forest* f = new Forest("C:\\Dropbox\\phdcode\\swc-all\\scaled\\cell11_x40_scaled.swc");
	if (!f->validate())
		return 5;
	Forest* g = new Forest(*f);
	if (!g->validate())
		return 10;
	g->reduce();
	if (!g->validate())
		return 15;
	Forest* h = f->generateForest([](Node* n, float& angle) -> bool {angle = 0.f; return true; }, vecN(0.f, 0.f, 1.f));
	if (!h->validate())
		return 20;
	h->write("C:\\Dropbox\\phdcode\\swc-all\\scaled\\cell11_x40_scaled_flat_test.swc");
	std::vector<Point> samples;
	h->samplePoints(samples, 10000);
	std::vector<float> samplesFlat;

	for (const auto& p : samples)
	{
		samplesFlat.push_back(p.x);
		samplesFlat.push_back(p.y);
		samplesFlat.push_back(p.z);
	}
	Eigen::Map<Eigen::MatrixXf> data(samplesFlat.data(),3,10000);
	Eigen::Vector3f v = data.col(0);
	Point p = samples[0];
	return (int)samples[500].x;
	//return g->getNode(1)->numChildren();
}