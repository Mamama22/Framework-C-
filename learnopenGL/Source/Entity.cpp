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
	parentTransforming = false;
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
	Added_ToEntity(parent);
}

/********************************************************************************
If associated with an entity, whether as a direct parent or further down
********************************************************************************/
void Entity::Added_ToEntity(Entity* addedTo)
{
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->Added_ToEntity(addedTo->handle);
}

void Entity::Removed()
{
	parent = NULL;
}

/********************************************************************************
Init func
********************************************************************************/
void Entity::Init(Vector3 pos, Vector3 scale)
{
	transform.Set(pos, scale);
	parentTransforming = false;
}

/********************************************************************************
Translate entity and children + components
No need to roate children by parent, their pos will be derived from TRS
********************************************************************************/
void Entity::Translate(Vector3 vel)
{
	transform.Translate(vel);

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
	//parent is transforming-----------------------//
	parentTransforming = true;

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
	//parent is transforming-----------------------//
	parentTransforming = true;

	transform.Translate_byParent(vel);

	//rotate all children by parent (angle, does not affect their TRS)-------------//
	for (int i = 0; i < children.size(); ++i)
		children[i]->ByParent_Translate(vel);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->ByParent_Translate(vel);
}

/********************************************************************************
Calculate overall TRS as well as for children, 
********************************************************************************/
void Entity::CalculateTRS()
{
	if (parent)	//if have parent, TRS calculated before
		return;

	transform.Calculate_TRS();

	for (int i = 0; i < children.size(); ++i)
		children[i]->CalculateTRS_WithParent(transform.TRS);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->CalculateTRS_WithParent(transform.TRS, parentTransforming);
}


/********************************************************************************
Rotate with entity (parent): when entity rotates, pos of this component
changes along the axis entity rotates
********************************************************************************/
void Entity::CalculateTRS_WithParent(const Mtx44& parentRotMat)
{
	Mtx44 tmp = transform.Calculate_TRS_withParent(parentRotMat);

	for (int i = 0; i < children.size(); ++i)
		children[i]->CalculateTRS_WithParent(tmp);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->CalculateTRS_WithParent(tmp, parentTransforming);
}

/********************************************************************************
pre update
********************************************************************************/
void Entity::PreUpdate()
{
	transform.PreUpdate();
	parentTransforming = false;
}

/********************************************************************************
Update if active
********************************************************************************/
void Entity::Update()
{
	UpdateEntity();
}


/********************************************************************************
Update entity
********************************************************************************/
void Entity::UpdateEntity()
{
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