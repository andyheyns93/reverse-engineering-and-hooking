#include "ThisCalculator.h"

ThisCalculator::ThisCalculator(float arg1, float arg2)
{
	m_arg1 = arg1;
	m_arg2 = arg2;
}
float ThisCalculator::GetAddition()
{
	float addition = m_arg1 + m_arg2;
	return addition;
}
float ThisCalculator::GetDivision()
{
	float division = m_arg1 / m_arg2;
	return division;
}
float ThisCalculator::GetMultiplication()
{
	float multiplication = m_arg1 * m_arg2;
	return multiplication;
}
float ThisCalculator::GetSubtraction()
{
	float subtraction = m_arg1 - m_arg2;
	return subtraction;
}