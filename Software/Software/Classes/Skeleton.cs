﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace Software.Classes
{
    public class Skeleton
    {
        List<Bone> bones;

        Bone ArmUp;
        Bone ArmDown;

        public bool isReady;

        public Skeleton(Controller c)
        {
            bones = new List<Bone>();
            ArmUp = new Bone();
            ArmDown = new Bone(ArmUp);
            bones.Add(ArmUp);
            bones.Add(ArmDown);
        }
        public Bone GetBone(int boneIndex = 0)
        {
            if (boneIndex > bones.Count - 1) { 
                Logger.Error("Out of bound bone selection"); return null;
            }

            return bones[boneIndex];
        }
        public int Reset()
        {
            isReady = false;
            return 0;
        }
        public int setupBone(int boneIndex, Sensor s, Bone b )
        {
            if (boneIndex > bones.Count - 1)
            {
                Logger.Error("Out of bound bone selection");  return 1;
            }

            bones[boneIndex].ConnctedSensor = s;
            bones[boneIndex].parentBone = b;
            isReady = true;
            return 0;
        }
        public void Calculate()
        {
            foreach (var bone in bones)
            {
               // Logger.Log($"{bone.Rot.X}");
                bone.Calculate();
            }
        }
    }
}
