﻿using System;
using System.IO;

#if UNITY 
using UnityEngine;
using UnityEngine.Networking;
#endif

namespace CSharpDemo
{
    class Program
    {
#if UNITY        
        public static void GetStreamingContent(string path, Action<string> cb)
        {
            string filePath = Path.Combine(Application.streamingAssetsPath, path);
    #if UNITY_ANDROID
            StartCoroutine(LoadAsset(filePath, cb));
    #else
            using (StreamReader reader = new StreamReader(filePath))
            {
                cb(reader.ReadToEnd());
            }
    #endif
        }

        IEnumerator LoadAsset(string filePath, Action<string> cb)
        {
            using (UnityWebRequest www = UnityWebRequest.Get(filePath))
            {
                yield return www.SendWebRequest();
                if (www.isNetworkError || www.isHttpError)
                {
                    Debug.LogErrorFormat("LoadAsset: error: {0} {1}",  www.error, filePath);
                    cb(null);
                }
                else
                {
                    cb(www.downloadHandler.data);
                }
            }
        }
#endif         
    
        static void ReadFileContent(string filepath, Action<string> cb)
        {
#if UNITY
            GetStreamingContent(filepath, cb);
#else
            string path = string.Format("../../../../res/{0}", filepath);
            StreamReader reader = new StreamReader(path);
            var content = reader.ReadToEnd();
            cb(content);    
#endif
        }

        static void onLoaded()
        {
            foreach (var item in Config.BoxProbabilityDefine.Data)
            {
                Console.WriteLine(item.ID);
                Console.WriteLine(item.Total);
                foreach (var elem in item.ProbabilityGoods)
                {
                    Console.WriteLine(string.Format("  {0} {1} {2}", elem.GoodsID, elem.Num, elem.Probability));
                }
            }
        }

        static void Main(string[] args)
        {
            try
            {
                Config.AutogenConfigManager.reader = ReadFileContent;
                Config.AutogenConfigManager.LoadAllConfig(() =>
                {
                    Console.WriteLine("OK");
                    onLoaded();
                });
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }
    }
}