#include "ExCommander.h"



bool ExStringPack::update(const AExactoPhysics::es_options_list new_type, const FString new_vec)
{
	if (new_type == AExactoPhysics::es_options_list::constraint_name)
	{
		Old = Target->name_constraint;
		Target->name_constraint = new_vec;
	}
	else if (new_type == AExactoPhysics::es_options_list::parent_name)
         	{
         		Old = Target->name_p;
         		Target->name_p = new_vec;
         	}
	else if (new_type == AExactoPhysics::es_options_list::target_name)
         	{
         		Old = Target->name_t;
         		Target->name_t = new_vec;
         	}
	else return false;
	return true;
}

bool ExStringPack::revert()
{
	if (Type == AExactoPhysics::es_options_list::constraint_name)
		Target->name_constraint = Old;
	else if (Type == AExactoPhysics::es_options_list::parent_name)
		Target->name_p = Old;
	else if (Type == AExactoPhysics::es_options_list::target_name)
		Target->name_t = Old;
	else return false;
	return true;
}

bool ExVectorPack::update(const AExactoPhysics::es_options_list new_type, const FVector new_vec)
{
	if (new_type == AExactoPhysics::es_options_list::dump_ang)
	{
		Old = Target->dump_ang;
		Target->dump_ang = new_vec;
	}
	else if (new_type == AExactoPhysics::es_options_list::dump_lin)
	{
		Old = Target->dump_lin;
		Target->dump_lin = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::low_lim_ang)
	{
		Old = Target->low_lim_ang;
		Target->low_lim_ang = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::low_lim_lin)
	{
		Old = Target->low_lim_lin;
		Target->low_lim_lin = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::parent_axis)
	{
		Old = Target->axis_p;
		Target->axis_p = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::parent_pivot)
	{
		Old = Target->pivot_p;
		Target->pivot_p = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::stiff_ang)
	{
		Old = Target->stiff_ang;
		Target->stiff_ang = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::stiff_lin)
	{
		Old = Target->stiff_lin;
		Target->stiff_lin = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::target_axis)
	{
		Old = Target->axis_t;
		Target->axis_t = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::target_pivot)
	{
		Old = Target->pivot_t;
		Target->pivot_t = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::upp_lim_ang)
	{
		Old = Target->upp_lim_ang;
		Target->upp_lim_ang = new_vec;		
	}
	else if (new_type == AExactoPhysics::es_options_list::upp_lim_lin)
	{
		Old = Target->upp_lim_lin;
		Target->upp_lim_lin = new_vec;		
	}
	else
		return false;
	Type = new_type;
	return true;	
}

bool ExVectorPack::revert()
{
		if (Type == AExactoPhysics::es_options_list::dump_ang)
    	{
    		Target->dump_ang = Old;
    	}
    	else if (Type == AExactoPhysics::es_options_list::dump_lin)
    	{
    		Target->dump_lin = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::low_lim_ang)
    	{
    		Target->low_lim_ang = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::low_lim_lin)
    	{
    		Target->low_lim_lin = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::parent_axis)
    	{
    		Target->axis_p = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::parent_pivot)
    	{
    		Target->pivot_p = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::stiff_ang)
    	{
    		Target->stiff_ang = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::stiff_lin)
    	{
    		Target->stiff_lin = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::target_axis)
    	{
    		Target->axis_t = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::target_pivot)
    	{
    		Target->pivot_t = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::upp_lim_ang)
    	{
    		Target->low_lim_ang = Old;		
    	}
    	else if (Type == AExactoPhysics::es_options_list::upp_lim_lin)
    	{
    		Target->low_lim_lin = Old;		
    	}
    	else
    		return false;
	return true;
}

