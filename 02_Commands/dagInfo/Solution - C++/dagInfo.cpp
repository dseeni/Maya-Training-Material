// 
// File: 
//
// Dependency Graph Node: 
//
// Author: Maya Plug-in Wizard 2.0
//
#include "dagInfo.h"

#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnDagNode.h>
#include <maya/MMatrix.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnTransform.h>

//- This method performs the action of the command. It iterates over all
//- selected items and prints out connected plug and dependency node type
//- information.
MStatus dagInfo::doIt(const MArgList& )
{
	MStatus stat;

	//- Select all objects currently selected into the Maya editor.
	MSelectionList slist;
	MGlobal::getActiveSelectionList( slist );
	//- Create an iterator on the selection list (using the iterator pattern).
	MItSelectionList iter( slist, MFn::kDagNode,&stat );

	//- Iterate over all selected dependency nodes
	for ( ; !iter.isDone(); iter.next() ) 
	{
		//- get the dependency node first and then apply MFnDagNode function set onto it.
		MObject depNode;
		iter.getDependNode(depNode);

		MFnDagNode fnDag(depNode);
		cout<<"********************************************************"<<endl;
		cout<<"The selected node name is "<<fnDag.name(&stat)<<", node type : "<<depNode.apiTypeStr()<<endl;

		//- Retrieve number of instances on this dag node
		unsigned int num = fnDag.instanceCount(true,&stat);
		if( num != 1 )	cout<<"Number of instances on this node is : "<<num<<endl;
		
		//- Retrieve all the instanced paths of this dag node and print out them
		MDagPathArray dagPathArray;
		fnDag.getAllPaths(dagPathArray);
		for(unsigned int j = 0; j < dagPathArray.length(); j++)
		{
			MDagPath instanceDagPath = dagPathArray[j];
			cout<<"Dag Path "<<j<<" for this node:"<<instanceDagPath.fullPathName(&stat)<<endl;
		
			//- Get the exclusive matrix of this node
			MMatrix exMatrix = instanceDagPath.exclusiveMatrix(&stat);
			cout<<"The exclusive transformation matrix of this node is "<<exMatrix<<endl;

			//- Get the inclusive matrix of this node
			//- If it is a shape node, the inclusive and exclusive matrix should be the same
			//- If it is a transform node and its transformation matrix is not identity, they 
			//- should be different!
			MMatrix inMatrix = instanceDagPath.inclusiveMatrix(&stat);
			cout<<"The inclusive transformation matrix of this node is "<<inMatrix<<endl;

			//- If this dag node is a transform node, also get its local transformation matrix
			if (depNode.hasFn(MFn::kTransform))
			{
				MFnTransform fnTrans(instanceDagPath);
				MTransformationMatrix localMatrix = fnTrans.transformation();
				cout<<"The local transformation matrix represented by this transform node is "<<localMatrix.asMatrix()<<endl;
			}
		}
	}

	//- Return success to Maya
	return MS::kSuccess;
}