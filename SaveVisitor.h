#pragma once
#include "Tips.h"
#include <iostream>
#include <fstream>

struct SaveVisitor : public Visitor
{
	SaveVisitor(const char* fileName) :  out(fileName, std::ios::out | std::ios::binary) {}
	~SaveVisitor() { out.close(); }

private:
	std::ofstream out;
public:
	virtual void visit_Turn(const Turn& tip) { out << "T" << tip.dir; }
	virtual void visit_Forward(const Forward& tip) {out << "F" << tip.n; }
	virtual void visit_ExitRamp(const ExitRamp& tip) {out << "E" << tip.n; }
	virtual void visit_Destination(const Destination &tip) {out << "D" << tip.dir; }
};

