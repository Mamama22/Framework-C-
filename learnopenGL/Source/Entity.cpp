#include "Entity.h"
#include "CoreUtilities.h"
Component* Entity::p;
vector<Component*>::iterator Entity::it;

/********************************************************************************
Constructor/destructor
********************************************************************************/
Entity::Entity()
{ 
	parent = NULL; 
	CU::entityMan.RegisterEntity(this);	//MUST REGISTER
	transforming = false;
	active = false;
}

Entity::~Entity()
{
	componentList.clear(); 
	children.clear(); 
}

/********************************************************************************
Add component
********************************************************************************/
void Entity::AddComponent(Component* comp)
{
	componentList.push_back(comp);
	comp->Added(transform, handle);

	if (active)
		comp->SetActive(true);
}

/********************************************************************************
Add child (Entity)
********************************************************************************/
void Entity::AddChildren(Entity* child)
{
	for (int i = 0; i < children.size(); ++i)
	{
		if (child == children[i])
			return;
	}

	children.push_back(child);
	child->Added(this);
}

/********************************************************************************
Remove component
********************************************************************************/
void Entity::RemoveComponent(Component* comp)
{
	for (int i = 0; i < componentList.size(); ++i)
	{
		if (comp == componentList[i])
		{
			componentList[i] = componentList.back();
			comp->Removed();
			componentList.pop_back();
			break;
		}
	}
}

/********************************************************************************
Remove child (Entity)
********************************************************************************/
void Entity::RemoveChildren(Entity* child)
{
	for (int i = 0; i < children.size(); ++i)
	{
		if (child == children[i])
		{
			children[i] = children.back();
			child->Removed();
			children.pop_back();
			break;
		}
	}
}

/********************************************************************************
if this entity added/removed, do something
CALL AT STAGE 1
********************************************************************************/
void Entity::Added(Entity* parent)
{
	this->parent = parent;
	
	//Do preparations for being a child-----------------------------------------//
	Entity* parent_ptr = parent;

	//Get the highest parent------------------//
	vector<Entity*> parentList;
	while (parent_ptr)
	{
		parentList.push_back(parent_ptr);
		parent_ptr = parent_ptr->parent;
	}

	//transform--------------------------//
	for (int i = parentList.size() - 1; i >= 0; --i)
		transform.AddedToParent(parentList[i]->transform);


	//apply action on all children and components-------------------------//
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->Added_ToGrandparent(parent->handle);
}

void Entity::Removed()
{
	//Do preparations for being a child-----------------------------------------//
	Entity* parent_ptr = parent;

	//Get the highest parent------------------//
	vector<Entity*> parentList;
	while (parent_ptr)
	{
		parentList.push_back(parent_ptr);
		parent_ptr = parent_ptr->parent;
	}

	//transform--------------------------//
	for (int i = 0; i < parentList.size(); ++i)
		transform.RemovedFromParent(parentList[i]->transform);

	//apply action on all children and components-------------------------//
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->RemovedFrom_Grandparent(parent->handle);

	parent = NULL;
}

/********************************************************************************
Init func
********************************************************************************/
void Entity::Init(Vector3 pos, Vector3 scale)
{
	transform.Set(pos, scale);
	transforming = false;
}

/********************************************************************************
Translate entity and children + components
No need to roate children by parent, their pos will be derived from TRS
********************************************************************************/
void Entity::Translate(Vector3 vel)
{
	transform.Translate(vel);
	transforming = true;

	//rotate all children by parent (angle, does not affect their TRS)-------------//
	for (int i = 0; i < children.size(); ++i)
		children[i]->ByParent_Translate(vel);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->ByParent_Translate(vel);
}

/********************************************************************************
Rotates entity
********************************************************************************/
void Entity::Rotate(float angle, Vector3 axis)
{
	transform.Rotate(angle, axis);
	transforming = true;

	//rotate all children by parent (angle, does not affect their TRS)-------------//
	for (int i = 0; i < children.size(); ++i)
		children[i]->ByParent_Rotate(angle, axis);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->ByParent_Rotate(angle, axis);
}

/********************************************************************************
Rotate by parent: does not affect TRS
********************************************************************************/
void Entity::ByParent_Rotate(float angle, Vector3 axis)
{
	transform.Rotate_byParent(angle, axis);

	//rotate all children by parent (angle, does not affect their TRS)-------------//
	for (int i = 0; i < children.size(); ++i)
		children[i]->ByParent_Rotate(angle, axis);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->ByParent_Rotate(angle, axis);
}

/********************************************************************************
Translate by parent: does not affect TRS and vec3 pos
********************************************************************************/
void Entity::ByParent_Translate(Vector3 vel)
{
	transform.Translate_byParent(vel);

	//rotate all children by parent (angle, does not affect their TRS)-------------//
	for (int i = 0; i < children.size(); ++i)
		children[i]->ByParent_Translate(vel);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->ByParent_Translate(vel);
}

/********************************************************************************
Calculate overall TRS as well as for children
Called by ELDEST ancestor
********************************************************************************/
void Entity::CalculateTRS()
{
	if (parent)	//if have parent, TRS calculated before
		return;

	//get the ID of transforming ancestor(THIS)-------------------------------//
	int transforming_ancestor = handle;
	if (!transforming)	//if this entity is not transforming, Transforming Ancestor = -1
		transforming_ancestor = -1;

	transform.Calculate_TRS();

	for (int i = 0; i < children.size(); ++i)
		children[i]->CalculateTRS_WithParent(transform.TRS, transforming_ancestor);	//transformBy_Ancestor_ID is handle of ELDEST(this)
	for (int i = 0; i < componentList.size(); ++i)	
		componentList[i]->CalculateTRS_WithParent(transform.TRS, -1);	//no ancestor, this is the ELDEST
}


/********************************************************************************
This can only be called if parent IS TRANSFORMING, so transformBy_Parent_ID wil never be -1
********************************************************************************/
void Entity::CalculateTRS_WithParent(const Mtx44& parentRotMat, int transformBy_Ancestor_ID)
{
	//get the ID of transforming ancestor-------------------------------//
	int transforming_ancestor = transformBy_Ancestor_ID;

	//check if there's a transforming ancestor-------------------------//
	if (transforming_ancestor != -1)
		this->transformBy_Ancestor_ID = transforming_ancestor;
	else if (transforming)
		transforming_ancestor = handle;	//THIS will become new the transforming ancestor

	//Make sure comp. will get Grandparent and above transforming handle-------------------------------------//
	int transforming_ancestor_not_parent = transforming_ancestor;
	if (transforming_ancestor_not_parent == handle)
		transforming_ancestor_not_parent = -1;

	Mtx44 tmp = transform.Calculate_TRS_withParent(parentRotMat);

	for (int i = 0; i < children.size(); ++i)
		children[i]->CalculateTRS_WithParent(tmp, transforming_ancestor);	//if parent(THIS) transform

	//Assumes grandparent is transforming, since child entity collision always affect grandparent, or else it shouldn't be
	//a child of grandparent's shape
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->CalculateTRS_WithParent(tmp, transforming_ancestor_not_parent);
}

/********************************************************************************
pre update
********************************************************************************/
void Entity::PreUpdate()
{
	transform.PreUpdate();
	transforming = false;
	transformBy_Ancestor_ID = -1;
}

/********************************************************************************
Update if active
********************************************************************************/
void Entity::Update_Stage1()
{
	
}

void Entity::Update_Stage2()
{
	CalculateTRS();
}

void Entity::Update_Stage3()
{

}


void Entity::Update_Stage4()
{
	CalculateTRS();
}


/********************************************************************************
Get
********************************************************************************/
Entity* Entity::GetParent(){ return parent; }

/********************************************************************************
Get
********************************************************************************/
Entity* Entity::GetTopParent()
{
	Entity* parent_ptr = this;

	//Get the highest parent------------------//
	while (parent_ptr->parent)
		parent_ptr = parent_ptr->parent;
	return parent_ptr;
}

void Entity::GetChildrenList(vector<Entity*>& list)
{
	for (int i = 0; i < children.size(); ++i)
		list[i] = children[i];
}

void Entity::GetCompList(vector<Component*>& list)
{
	for (int i = 0; i < componentList.size(); ++i)
		list[i] = componentList[i];
}

int Entity::Gethandle(){ return handle; }
void Entity::Sethandle(int handle){ this->handle = handle; }

bool Entity::getActive(){ return active; }
void Entity::SetActive(bool b)
{ 
	active = b; 

	//children and comp set active
	for (int i = 0; i < children.size(); ++i)
		children[i]->SetActive(b);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->SetActive(b);
}