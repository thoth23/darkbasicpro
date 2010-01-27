
#include "PhysXWrapper.h"
#include "UpdateTime.h"
#include "NxCooking.h"
#include "stream.h"
#include "D:\Coding\Dark Basic Pro Shared\Dark Basic Pro SDK\SHARED\DBOFORMAT\DBOData.h"
#include "UserAllocator.h"

#include "NxController.h"
#include "NxBoxController.h"
#include "NxSphereController.h"

#include "DarkSDK.h"

NxPhysicsSDK*     gPhysicsSDK = NULL;
NxScene*          gScene      = NULL;
NxVec3            gDefaultGravity ( 0, -9.8, 0 );

sObject* dbGetObject ( int iID );
void SetWorldMatrix ( int iID, D3DXMATRIX* pMatrix );

bool GetFVFOffsetMap ( sMesh* pMesh, sOffsetMap* psOffsetMap );

enum GameGroup
{
    GROUP_NON_COLLIDABLE,
    GROUP_COLLIDABLE_NON_PUSHABLE,
    GROUP_COLLIDABLE_PUSHABLE,
};

NxActor* box [ 100 ];
UserAllocator*	  gAllocator = NULL;

/////////////////////////////////
const bool frontWheelIsPowered = true;
const bool rearWheelIsPowered = false;
const NxReal suspSpring = 70; 
const NxReal suspDamping = 5; 
const NxReal suspBias = -0.25; 
const NxReal carMass = 30;

NxReal gSteeringDelta = 0.05;
NxReal gMaxMotorPower = 40;
NxReal gMaxSteeringAngle = NxMath::degToRad(45.0f);
NxReal gMotorForce;
NxReal gSteeringValue = 0;

NxMaterialIndex raycastWheelMaterialIndex;

NxActor* car;



struct CarWheelContact
{
	NxActor* car;
	NxShape* wheel;
	NxVec3 contactPoint;
	NxVec3 contactNormalForce;
	NxVec3 contactFrictionForce;
};

NxArray<CarWheelContact> wheelContactPoints;

struct WheelShapeUserData
{
	bool frontWheel;
} wheelUserDatas[4];

NxMaterialDesc raycastWheelMaterial;
NxSpringDesc wheelSpring;

struct CarData
{
	NxReal _;	//add stuff later maybe.
} carData;
/////////////////////////////////

struct sPhysObject
{
	int iID;
};


bool isCar(NxActor * actor)
{
	sPhysObject* pPhys = (sPhysObject *)(actor->userData);

	if ( pPhys == NULL )
		return false;

	if ( pPhys->iID == 5 )
		return true;

	return false;

	//return ((&carData) == (CarData *)(actor->userData));
	
}

class ContactReport : public NxUserContactReport
{
public:

	virtual void  onContactNotify(NxContactPair& pair, NxU32 events)
	{
		
		NxU32 carIndex = 0;
		if (isCar(pair.actors[0]))
			carIndex = 0;
		else if (isCar(pair.actors[1]))
			carIndex = 1;
		else
			return;
		
		//ignore the 'both are cars' case for now.

		// Iterate through contact points
		NxContactStreamIterator i(pair.stream);
		//user can call getNumPairs() here
		while (i.goNextPair())
        {
			//user can also call getShape() and getNumPatches() here
			NxShape * s = i.getShape(carIndex);
			while (i.goNextPatch())
            {
				//user can also call getPatchNormal() and getNumPoints() here
				const NxVec3& contactNormal = i.getPatchNormal();
				while (i.goNextPoint())
                {
					//user can also call getPoint() and getSeparation() here
					const NxVec3& contactPoint = i.getPoint();

					NxVec3 contactNormalForce = pair.sumNormalForce;
					NxVec3 contactFrictionForce = pair.sumFrictionForce;

					//add forces:

					//assuming front wheel drive we need to apply a force at the wheels.
					if (s->is(NX_SHAPE_CAPSULE))		//assuming only the wheels of the car are capsules, otherwise we need more checks.
														//this branch can't be pulled out of loops because we have to do a full iteration through the stream
                    {
						CarWheelContact cwc;
						cwc.car = pair.actors[carIndex];
						cwc.wheel = s;
						cwc.contactPoint = contactPoint;
						cwc.contactNormalForce = contactNormalForce;
						cwc.contactFrictionForce = contactFrictionForce;
						wheelContactPoints.pushBack(cwc);
						//#error too bad this is illegal (reentry) and also technically busted because the accumulators get zeroed after this returns.
						//pair.actors[carIndex]->addForceAtPos(NxVec3(100,0,0),contactPoint);
                    }
                }
            }		
        }
	}
} carContactReportObj;

NxUserContactReport* carContactReport = &carContactReportObj;


void PhysStart ( void )
{
	gAllocator = new UserAllocator;
    gPhysicsSDK = NxCreatePhysicsSDK ( NX_PHYSICS_SDK_VERSION,gAllocator );

    if ( !gPhysicsSDK )
		return;

	gPhysicsSDK->setParameter ( NX_MIN_SEPARATION_FOR_PENALTY, -0.05f );

	gPhysicsSDK->setParameter ( NX_VISUALIZATION_SCALE,        1 );
	gPhysicsSDK->setParameter ( NX_VISUALIZE_COLLISION_SHAPES, 1 );
	gPhysicsSDK->setParameter ( NX_VISUALIZE_ACTOR_AXES,       1 );
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_FNORMALS, 1);

    NxSceneDesc			sceneDesc;
    sceneDesc.gravity               = gDefaultGravity;
    sceneDesc.broadPhase            = NX_BROADPHASE_COHERENT;
    sceneDesc.collisionDetection    = true;
	sceneDesc.userContactReport		= carContactReport;
    gScene = gPhysicsSDK->createScene ( sceneDesc );

	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex ( 0 );
	defaultMaterial->setRestitution     ( 0.0f );
	defaultMaterial->setStaticFriction  ( 0.5f );
	defaultMaterial->setDynamicFriction ( 0.5f );

	gPhysicsSDK->setActorGroupPairFlags(0, 0, NX_NOTIFY_ON_TOUCH);
}


void SetActorCollisionGroup ( NxActor* actor, NxCollisionGroup group )
{
    NxShape** shapes = actor->getShapes ( );
    NxU32 nShapes = actor->getNbShapes ( );

    while ( nShapes-- )
    {
        shapes [ nShapes ]->setGroup ( group );
	}
}

void PhysCreateStaticRigidBodyMesh ( int iObject )
{
	sObject* pObject = dbGetObject ( iObject );

	if ( !pObject )
		return;

	int iCount = 0;
	
	for ( int iMesh = 0; iMesh < pObject->iMeshCount; iMesh++ )
	{
		NxVec3* pVertices		= new NxVec3 [ pObject->ppMeshList [ iMesh ]->dwVertexCount ];
		int		iVertexCount	= pObject->ppMeshList [ iMesh ]->dwVertexCount;
		int*	iTriangles		= new int [ pObject->ppMeshList [ iMesh ]->dwIndexCount ];
		int		iTriangleCount	= pObject->ppMeshList [ iMesh ]->dwIndexCount;

		sOffsetMap offsetMap;
		GetFVFOffsetMap ( pObject->ppMeshList [ iMesh ], &offsetMap );

		for ( int i = 0; i < iVertexCount; i++ )
		{
			pVertices [ i ].x = *( ( float* ) pObject->ppMeshList [ iMesh ]->pVertexData + offsetMap.dwX + ( offsetMap.dwSize * i ) );
			pVertices [ i ].y = *( ( float* ) pObject->ppMeshList [ iMesh ]->pVertexData + offsetMap.dwY + ( offsetMap.dwSize * i ) );
			pVertices [ i ].z = *( ( float* ) pObject->ppMeshList [ iMesh ]->pVertexData + offsetMap.dwZ + ( offsetMap.dwSize * i ) );
		}

		for ( i = 0; i < iTriangleCount; i++ )
			iTriangles [ i ] = pObject->ppMeshList [ iMesh ]->pIndices [ i ];
		
		NxTriangleMeshDesc levelDesc;
		levelDesc.numVertices			= iVertexCount;
		levelDesc.numTriangles			= iTriangleCount / 3;
		levelDesc.pointStrideBytes		= sizeof ( NxVec3 );
		levelDesc.triangleStrideBytes   = 3 * sizeof ( int );
		levelDesc.points				= pVertices;
		levelDesc.triangles				= iTriangles;
		levelDesc.flags					= NX_CF_COMPUTE_CONVEX;

		NxTriangleMeshShapeDesc levelShapeDesc;
		NxInitCooking ( );
		
		MemoryWriteBuffer buf;
		bool status = NxCookTriangleMesh ( levelDesc, buf );
		
		if ( status )
		{
			levelShapeDesc.meshData = gPhysicsSDK->createTriangleMesh ( MemoryReadBuffer ( buf.data ) );
		
			NxActor* actor = NULL;

			NxActorDesc actorDesc;
			actorDesc.shapes.pushBack ( &levelShapeDesc );

			actor = gScene->createActor ( actorDesc );
			
			sPhysObject* pPhys = new sPhysObject;
			pPhys->iID      = iObject;
			actor->userData = ( void* )pPhys;

			SetActorCollisionGroup ( actor, GROUP_COLLIDABLE_NON_PUSHABLE );
		}
	}
}


void PhysCreateRigidBodyBox ( int iObject )
{
	sObject* pObject = dbGetObject ( iObject );

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	float fXPos	 = pObject->position.vecPosition.x;
	float fYPos	 = pObject->position.vecPosition.y;
	float fZPos	 = pObject->position.vecPosition.z;
	float fXSize = ( pObject->collision.vecMax.x - pObject->collision.vecMin.x ) * pObject->position.vecScale.x;
	float fYSize = ( pObject->collision.vecMax.y - pObject->collision.vecMin.y ) * pObject->position.vecScale.y;
	float fZSize = ( pObject->collision.vecMax.z - pObject->collision.vecMin.z ) * pObject->position.vecScale.z;

	// the actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set ( fXSize / 2, fYSize / 2, fZSize / 2 );

	boxDesc.localPose.t = NxVec3 ( pObject->collision.vecCentre.x, pObject->collision.vecCentre.y, pObject->collision.vecCentre.z  );

	actorDesc.shapes.pushBack ( &boxDesc );

	actorDesc.body = &bodyDesc;
	actorDesc.density = 10;
	actorDesc.globalPose.t = NxVec3 ( fXPos, fYPos, fZPos );
	box [ iObject ] = gScene->createActor ( actorDesc );

	sPhysObject* pPhys = new sPhysObject;
	pPhys->iID = iObject;
	box [ iObject ]->userData = (void*)pPhys;
}

void PhysSetVelocity ( int iObject, float fX, float fY, float fZ )
{
	box [ iObject ]->setLinearVelocity ( NxVec3 ( fX, fY, fZ ) );
}

void PhysCreateStaticRigidBodyBox ( int iObject )
{
	sObject* pObject = dbGetObject ( iObject );

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	float fXPos	 = pObject->position.vecPosition.x;
	float fYPos	 = pObject->position.vecPosition.y;
	float fZPos	 = pObject->position.vecPosition.z;
	float fXSize = ( pObject->collision.vecMax.x - pObject->collision.vecMin.x ) * pObject->position.vecScale.x;
	float fYSize = ( pObject->collision.vecMax.y - pObject->collision.vecMin.y ) * pObject->position.vecScale.y;
	float fZSize = ( pObject->collision.vecMax.z - pObject->collision.vecMin.z ) * pObject->position.vecScale.z;

	// the actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set ( fXSize, fYSize, fZSize );
	boxDesc.localPose.t = NxVec3 ( 0, 0, 0 );
	actorDesc.shapes.pushBack ( &boxDesc );

	//actorDesc.body = &bodyDesc;
	actorDesc.density = 10;
	actorDesc.globalPose.t = NxVec3 ( fXPos, fYPos, fZPos );
	box [ iObject ] = gScene->createActor ( actorDesc );

	sPhysObject* pPhys = new sPhysObject;
	pPhys->iID = iObject;
	box [ iObject ]->userData = (void*)pPhys;
}

void PhysRun ( void )
{
	// Update the time step
	NxReal deltaTime = UpdateTime();

	// Run collision and dynamics for delta time since the last frame
	gScene->simulate(deltaTime);	
	gScene->flushStream();
	gScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
}

DARKSDK_DLL void SetWorldMatrix ( int iID, D3DXMATRIX* pMatrix );

void PhysUpdate ( void )
{
	int iObject = 1;

    // Render all the actors in the scene
    int nbActors = gScene->getNbActors();
    NxActor** actors = gScene->getActors();

	while (nbActors--)
    {
        NxActor* actor = *actors++;

		// Get an OpenGL transform (float[16]) from a Novodex shape’s global pose     
		// (NxMat34)
		NxShape* shape = NULL;

		NxMat34 pose = actor->getGlobalPose();

		NxMat33 orient = pose.M;
		NxVec3 pos = pose.t;	

		float glmat[16];    // 4x4 column major OpenGL matrix
		orient.getColumnMajorStride4(&(glmat[0]));
		pos.get(&(glmat[12]));

		// clear the elements we don't need:
		glmat[3] = glmat[7] = glmat[11] = 0.0f;
		glmat[15] = 1.0f;

		{
			sPhysObject* pPhys = ( sPhysObject* ) actor->userData;
			
			if ( pPhys )
			{
				SetWorldMatrix ( pPhys->iID, ( D3DXMATRIX* ) &glmat );

				sObject* pObject = dbGetObject ( pPhys->iID );

				pObject->position.vecPosition = D3DXVECTOR3 ( glmat [ 12 ], glmat [ 13 ], glmat [ 14 ] );
			}
		}
    }
}

int g_iValue = 10;

void TickCar ( void )
{
	g_iValue = 10;

	NxReal steeringAngle = gSteeringValue * gMaxSteeringAngle;

	NxArray<CarWheelContact>::iterator i = wheelContactPoints.begin();
	while(i != wheelContactPoints.end())
	{
		CarWheelContact& cwc = *i;

		WheelShapeUserData* wheelData = (WheelShapeUserData *)(cwc.wheel->userData);

		/*
		struct CarWheelContact
		{
			NxActor* car;
			NxShape* wheel;
			NxVec3 contactPoint;
			NxVec3 contactNormalForce;
			NxVec3 contactFrictionForce;
		};
		*/

		{
			NxMat34 pose   = cwc.wheel->getGlobalPose ( );
			NxMat33 orient = pose.M;
			NxVec3  pos    = pose.t;

			float glmat[16];
			orient.getColumnMajorStride4(&(glmat[0]));
			pos.get(&(glmat[12]));
			glmat[3] = glmat[7] = glmat[11] = 0.0f;
			glmat[15] = 1.0f;

			SetWorldMatrix ( g_iValue, ( D3DXMATRIX* ) &glmat );
			sObject* pObject = dbGetObject ( g_iValue );
			pObject->position.vecPosition = D3DXVECTOR3 ( glmat [ 12 ], glmat [ 13 ], glmat [ 14 ] );

			//dbPositionObject ( g_iValue, glmat [ 12 ], glmat [ 13 ], glmat [ 14 ] );

			g_iValue++;
		}

		//apply to powered wheels only.
		if (wheelData->frontWheel)
	    {
			//steering:
			NxMat33 wheelOrientation = cwc.wheel->getLocalOrientation();
			wheelOrientation.setColumn(0,  NxVec3(NxMath::cos(steeringAngle), 0,  NxMath::sin(steeringAngle) ));
			wheelOrientation.setColumn(2,  NxVec3(NxMath::sin(steeringAngle), 0, -NxMath::cos(steeringAngle) ));
			cwc.wheel->setLocalOrientation(wheelOrientation);

			if (frontWheelIsPowered)
			{
				//get the world space orientation:
				wheelOrientation = cwc.wheel->getGlobalOrientation();
				NxVec3 steeringDirection;
				wheelOrientation.getColumn(0, steeringDirection);

				//the power direction of the front wheel is the wheel's axis as it is steered.
				if (gMotorForce)
				{
					cwc.car->addForceAtPos(steeringDirection * gMotorForce,cwc.contactPoint);
				}
			}
		}
		if (!wheelData->frontWheel && rearWheelIsPowered)
		{
			//get the orientation of this car:
			NxMat33 m = cwc.car->getGlobalOrientation();
			NxVec3 carForwardAxis;
			m.getColumn(0, carForwardAxis);
			//the power direction of the rear wheel is always the car's length axis.
			cwc.car->addForceAtPos(carForwardAxis * gMotorForce,cwc.contactPoint);
		}
		i++;
	}

	wheelContactPoints.clear();
}


NxActor* CreateCar(const NxVec3& pos)
{
	// Create body
	NxBodyDesc bodyDesc;
	NxBoxShapeDesc boxDesc;
	NxCapsuleShapeDesc capsuleDesc[4];
	NxActorDesc actorDesc;

	bodyDesc.mass = carMass;

	const NxReal height = 0.3;
	const NxReal width = 2;
	const NxReal length = 4;

	//boxDesc.dimensions.set(length*0.65, height*0.85, width*0.65);
	//actorDesc.shapes.pushBack(&boxDesc);

	{
		sObject* pObject = dbGetObject ( 5 );
	
		float fXPos	 = pObject->position.vecPosition.x;
		float fYPos	 = pObject->position.vecPosition.y;
		float fZPos	 = pObject->position.vecPosition.z;
		float fXSize = ( pObject->collision.vecMax.x - pObject->collision.vecMin.x ) * pObject->position.vecScale.x;
		float fYSize = ( pObject->collision.vecMax.y - pObject->collision.vecMin.y ) * pObject->position.vecScale.y;
		float fZSize = ( pObject->collision.vecMax.z - pObject->collision.vecMin.z ) * pObject->position.vecScale.z;

		boxDesc.dimensions.set ( fXSize / 2, fYSize / 2, fZSize / 2 );
		boxDesc.localPose.t = NxVec3 ( pObject->collision.vecCentre.x, pObject->collision.vecCentre.y, pObject->collision.vecCentre.z  );
		//boxDesc.localPose.
		actorDesc.shapes.pushBack ( &boxDesc );
	}

	capsuleDesc[0].radius = 0.1;
	capsuleDesc[0].height = 1;
	capsuleDesc[0].flags = NX_SWEPT_SHAPE;
	capsuleDesc[0].localPose.M.setColumn(1, NxVec3(0,-1, 0));
	capsuleDesc[0].localPose.M.setColumn(2, NxVec3(0, 0,-1));	//rotate 180 degrees around x axis to cast downward!
	capsuleDesc[0].localPose.t.set(length*0.5, -height, width*0.5);
	capsuleDesc[0].materialIndex = raycastWheelMaterialIndex;
	capsuleDesc[0].userData = &(wheelUserDatas[0]);
	actorDesc.shapes.pushBack(&capsuleDesc[0]);
	wheelUserDatas[0].frontWheel = true;

	capsuleDesc[1].radius = capsuleDesc[0].radius;
	capsuleDesc[1].height = capsuleDesc[0].height;
	capsuleDesc[1].flags = capsuleDesc[0].flags;
	capsuleDesc[1].localPose.M = capsuleDesc[0].localPose.M;	
	capsuleDesc[1].localPose.t.set(length*0.5, -height, -width*0.5);
	capsuleDesc[1].materialIndex = raycastWheelMaterialIndex;
	capsuleDesc[1].userData = &(wheelUserDatas[1]);
	actorDesc.shapes.pushBack(&capsuleDesc[1]);
	wheelUserDatas[1].frontWheel = true;

	capsuleDesc[2].radius = capsuleDesc[0].radius;
	capsuleDesc[2].height = capsuleDesc[0].height;
	capsuleDesc[2].flags = capsuleDesc[0].flags;
	capsuleDesc[2].localPose.M = capsuleDesc[0].localPose.M;
	capsuleDesc[2].localPose.t.set(-length*0.5, -height, -width*0.5);
	capsuleDesc[2].materialIndex = raycastWheelMaterialIndex;
	capsuleDesc[2].userData = &(wheelUserDatas[2]);
	actorDesc.shapes.pushBack(&capsuleDesc[2]);
	wheelUserDatas[2].frontWheel = false;

	capsuleDesc[3].radius = capsuleDesc[0].radius;
	capsuleDesc[3].height = capsuleDesc[0].height;
	capsuleDesc[3].flags = capsuleDesc[0].flags;
	capsuleDesc[3].localPose.M = capsuleDesc[0].localPose.M;
	capsuleDesc[3].localPose.t.set(-length*0.5, -height, width*0.5);
	capsuleDesc[3].materialIndex = raycastWheelMaterialIndex;
	capsuleDesc[3].userData = &(wheelUserDatas[3]);
	actorDesc.shapes.pushBack(&capsuleDesc[3]);
	wheelUserDatas[3].frontWheel = false;

	actorDesc.body			= &bodyDesc;
	actorDesc.globalPose.t  = pos;
	NxActor* a = gScene->createActor(actorDesc);

	//a->userData = &carData;		//so we can recoginze it in the contact stream

sPhysObject* pPhys = new sPhysObject;
pPhys->iID = 5;
a->userData = (void*)pPhys;

	//actorDesc.globalPose.t = NxVec3 ( fXPos, fYPos, fZPos );
NxMat33 mat;
mat.rotY ( D3DXToRadian ( 90 ) );
a->setGlobalOrientation ( mat );

	a->wakeUp(1e10);			//don't go to sleep.

	return a;
}

void InitCar()
{
	wheelSpring.spring =  suspSpring;
	wheelSpring.damper =  suspDamping;
	wheelSpring.targetValue = suspBias;

	raycastWheelMaterial.restitution		= 0.0;
	raycastWheelMaterial.dynamicFriction	= 0.8;
	raycastWheelMaterial.staticFriction		= 2.0;
	raycastWheelMaterial.staticFrictionV	= 0.5;
	raycastWheelMaterial.dynamicFrictionV	= 0.5;
	raycastWheelMaterial.dirOfAnisotropy.set(1, 0, 0);
	raycastWheelMaterial.frictionCombineMode = NX_CM_MULTIPLY;
	raycastWheelMaterial.flags			|=(NX_MF_SPRING_CONTACT | NX_MF_ANISOTROPIC);
	raycastWheelMaterial.programData	= &wheelSpring;
	raycastWheelMaterial.programDataSize = sizeof(wheelSpring);
	raycastWheelMaterialIndex = gScene->createMaterial(raycastWheelMaterial)->getMaterialIndex();
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

NxMaterialIndex wheelMaterialIndex = 1;


class Truck
{
public:

	Truck(): force(0), steerAngle(0) {}

	struct Wheel
	{
		void create(NxScene& scene, const NxVec3& pos, float rad, NxActor* holder)
		{
			NxActorDesc actorDesc;
			NxBodyDesc bodyDesc;
			NxSphereShapeDesc sphereDesc;
			
			bodyDesc.solverIterationCount = 20;

			// wheel
			sphereDesc.radius = rad;
			sphereDesc.materialIndex = wheelMaterialIndex;
			actorDesc.shapes.pushBack(&sphereDesc);
			bodyDesc.mass = 400;
			actorDesc.body = &bodyDesc;
			actorDesc.globalPose.t = pos;
			wheel = scene.createActor(actorDesc);

			// roll axis
			bodyDesc.mass = 50;
			bodyDesc.massSpaceInertia = NxVec3(1,1,1);
			actorDesc.body = &bodyDesc;
			actorDesc.shapes.clear();
			actorDesc.globalPose.t  = pos;
			rollAxis = scene.createActor(actorDesc);
			
			// revolute joint connecting wheel with rollAxis
			NxRevoluteJointDesc revJointDesc;
			revJointDesc.projectionMode = NX_JPM_POINT_MINDIST;
			revJointDesc.actor[0] = wheel;
			revJointDesc.actor[1] = rollAxis;
			revJointDesc.setGlobalAnchor(pos);
			revJointDesc.setGlobalAxis(NxVec3(0,0,1));
			rollJoint = (NxRevoluteJoint*)scene.createJoint(revJointDesc);

			// prismatic joint connecting rollAxis with holder
			NxPrismaticJointDesc prisJointDesc;
			prisJointDesc.actor[0] = rollAxis;
			prisJointDesc.actor[1] = holder;
			prisJointDesc.setGlobalAnchor(pos);
			prisJointDesc.setGlobalAxis(NxVec3(0,1,0));
			scene.createJoint(prisJointDesc);

			// add springs and dampers to the suspension (i.e. the related actors)
			float springLength = 0.1f;
			NxSpringAndDamperEffector * springNdamp = scene.createSpringAndDamperEffector(NxSpringAndDamperEffectorDesc());

			springNdamp->setBodies(rollAxis, pos, holder, pos + NxVec3(0,springLength,0));
			springNdamp->setLinearSpring(0, springLength, 2*springLength, 100000, 100000);
			springNdamp->setLinearDamper(-1, 1, 1e5, 1e5);

			// disable collision detection 
			scene.setActorPairFlags(*wheel, *holder, NX_IGNORE_PAIR);
		}

		NxActor* wheel;
		NxActor* rollAxis;
		NxRevoluteJoint* rollJoint;
	};


	struct SteerWheel
	{
		void create(NxScene& scene, const NxVec3& pos, float rad, NxActor* holder)
		{
			NxActorDesc actorDesc;
			NxBodyDesc bodyDesc;

			bodyDesc.solverIterationCount = 20;

			// steer axis
			bodyDesc.mass = 50;
			bodyDesc.massSpaceInertia = NxVec3(1,1,1);
			actorDesc.body = &bodyDesc;
			actorDesc.shapes.clear();

			actorDesc.globalPose.t = pos;
			steerAxis = scene.createActor(actorDesc);
			wheel.create(scene, pos, rad, steerAxis);

			// revolute joint connecting steerAxis with the holder
			NxRevoluteJointDesc revJointDesc;
			revJointDesc.projectionMode = NX_JPM_POINT_MINDIST;
			revJointDesc.actor[0] = steerAxis;
			revJointDesc.actor[1] = holder;
			revJointDesc.setGlobalAnchor(pos);
			revJointDesc.setGlobalAxis(NxVec3(0,1,0));
			steerJoint = (NxRevoluteJoint*)scene.createJoint(revJointDesc);

			// disable collision detection 
			scene.setActorPairFlags(*wheel.wheel, *holder, NX_IGNORE_PAIR);
		}
		
		Wheel wheel;	
		NxActor* steerAxis;
		NxRevoluteJoint* steerJoint;
	};

	void create(NxScene& scene )
	{
		NxBodyDesc bodyDesc;	
		NxBoxShapeDesc boxDesc0;
		NxBoxShapeDesc boxDesc1;

		bodyDesc.solverIterationCount = 20;
		
		// tractor actor (two shapes)
		NxActorDesc tractorDesc;
		bodyDesc.mass = 5000;
		tractorDesc.body = &bodyDesc;
		tractorDesc.globalPose.t = NxVec3(0,0,0);

		boxDesc0.dimensions = NxVec3(3.5,3,4);
		boxDesc0.localPose.t = NxVec3(3.5,4,0);
		tractorDesc.shapes.pushBack(&boxDesc0);
		
	

		//boxDesc1.dimensions = NxVec3(8.5,0.7,3);
		//boxDesc1.localPose.t = NxVec3(8.7,1.3,0);
		//tractorDesc.shapes.pushBack(&boxDesc1);

		tractor = scene.createActor(tractorDesc);

		//sPhysObject* pPhys = new sPhysObject;
		//pPhys->iID      = 5;
		//tractor->userData = ( void* )pPhys;

		// remove collision between lower truck parts and ground
		//scene.setShapePairFlags(*tractor->getShapes()[1], *ground.getShapes()[0], NX_IGNORE_PAIR);
		//scene.setShapePairFlags(*trailer->getShapes()[1], *ground.getShapes()[0], NX_IGNORE_PAIR);

		// wheels
		float staticFriction  = 1.2f;
		float dynamicFriction = 1.0f;

		steerWheels[0].create(scene, NxVec3(3.5,1.5,4), 1.5, tractor);
		steerWheels[1].create(scene, NxVec3(3.5,1.5,-4), 1.5, tractor);
		frontWheels[0].create(scene, NxVec3(16,1.5,4), 1.5, tractor);
		frontWheels[1].create(scene, NxVec3(16,1.5,-4), 1.5, tractor);
	}

	void control(bool keyLeft, bool keyRight, bool keyUp, bool keyDown, float dt)
	{
		const NxReal maxSteerAngle = 30.0f;
		NxReal sangle; 

		if (keyLeft ) steerAngle += 1.0f;
		if (keyRight) steerAngle -= 1.0f;
		
		if (steerAngle > 0.0) 
			sangle = min(steerAngle,  maxSteerAngle);
		else 
			sangle = max(steerAngle, -maxSteerAngle);	
		steerAngle = sangle;

		steerWheels[0].steerJoint->setSpring(NxSpringDesc(5e4, 100, NxMath::degToRad(sangle)));
		steerWheels[1].steerJoint->setSpring(NxSpringDesc(5e4, 100, NxMath::degToRad(sangle)));
		
		float gdvAcc = dt * 30.0f;
		float gdvBrk = dt * 10.0f;

		if (keyUp && !keyDown) 
			force += gdvAcc;
		else if (!keyUp &&  keyDown) 
			force -= gdvBrk;				
		else 
			force = 0.0f;

		unsigned i;
		float maxvel = NxMath::sign(force) * 1e5f;
		for (i=0; i<2; i++)
		{
			frontWheels[i].rollJoint->setMotor(NxMotorDesc(maxvel, 1.0f * NxMath::abs(force), false));
		}
		for (i=0; i<2; i++)
		{
			steerWheels[i].wheel.rollJoint->setMotor(NxMotorDesc(maxvel, 1.0f * NxMath::abs(force), false));
		}
	}

	void Update ( void )
	{
		NxVec3 forceDir ( -1, 0, 0 );
		NxReal forceStrength = 100;
		NxVec3 forceVec = forceStrength*forceDir;
		this->tractor->addForce(forceVec);

		g_iValue = 10;

		for (int i=0; i<2; i++)
		{
			NxMat34 pose   = frontWheels [ i ].wheel->getGlobalPose ( );
			NxMat33 orient = pose.M;
			NxVec3  pos    = pose.t;

			float glmat[16];
			orient.getColumnMajorStride4(&(glmat[0]));
			pos.get(&(glmat[12]));
			glmat[3] = glmat[7] = glmat[11] = 0.0f;
			glmat[15] = 1.0f;

			SetWorldMatrix ( g_iValue, ( D3DXMATRIX* ) &glmat );
			sObject* pObject = dbGetObject ( g_iValue++ );
			pObject->position.vecPosition = D3DXVECTOR3 ( glmat [ 12 ], glmat [ 13 ], glmat [ 14 ] );
		}

		for (int i=0; i<2; i++)
		{
			NxMat34 pose   = steerWheels [ i ].wheel.wheel->getGlobalPose ( );
			NxMat33 orient = pose.M;
			NxVec3  pos    = pose.t;

			float glmat[16];
			orient.getColumnMajorStride4(&(glmat[0]));
			pos.get(&(glmat[12]));
			glmat[3] = glmat[7] = glmat[11] = 0.0f;
			glmat[15] = 1.0f;

			SetWorldMatrix ( g_iValue, ( D3DXMATRIX* ) &glmat );
			sObject* pObject = dbGetObject ( g_iValue++ );
			pObject->position.vecPosition = D3DXVECTOR3 ( glmat [ 12 ], glmat [ 13 ], glmat [ 14 ] );
		}

		//dbPositionObject ( g_iValue, glmat [ 12 ], glmat [ 13 ], glmat [ 14 ] );

	
	}

	NxActor* tractor;
	NxActor* trailer;
	SteerWheel steerWheels[2];
	Wheel frontWheels[2];
	Wheel backWheels[4];

	float force;
	float steerAngle;
};


Truck* truck = NULL;

void CreateTruck ( void )
{
	truck = new Truck();
	truck->create(*gScene);
}

void UpdateTruck ( void )
{
	truck->Update ( );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////