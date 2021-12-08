#pragma once
class ThisCalculator
{
private:
	float m_arg1;
	float m_arg2;
public:
	ThisCalculator(float arg1, float arg2);
	float GetAddition();
	float GetDivision();
	float GetMultiplication();
	float GetSubtraction();
};

