
namespace GoogleARCore.HelloAR
{
    using System.Collections.Generic;
    using UnityEngine;
    using UnityEngine.Rendering;
    using GoogleARCore;
    using UnityEngine.SceneManagement;

    public class HelloARController : MonoBehaviour
    {

        

        public void ScenechangeToMain()
        {
            Reset1();
            SceneManager.LoadScene("Menu");
        }


        public Camera m_firstPersonCamera;



        public GameObject m_trackedPlanePrefab;

        public GameObject m_andyAndroidPrefab;

        public GameObject m_2andyAndroidPrefab;

        public GameObject m_3andyAndroidPrefab;

        public GameObject m_4andyAndroidPrefab;

        public GameObject m_5andyAndroidPrefab;

        int ObjectIndex = 0;


        public GameObject m_searchingForPlaneUI;

        private List<TrackedPlane> m_newPlanes = new List<TrackedPlane>();

        private List<TrackedPlane> m_allPlanes = new List<TrackedPlane>();

        private Color[] m_planeColors = new Color[] {
            new Color(1.0f, 1.0f, 1.0f),

            new Color(1.0f, 0.756f, 0.027f)
        };

        


        public void purpleClickAndy()
        {
            ObjectIndex = 1;
        }

        public void purpleClick2Andy()
        {
            ObjectIndex = 2;
        }

        public void purpleClick3Andy()
        {
            ObjectIndex = 3;
        }

        public void purpleClick4Andy()
        {
            ObjectIndex = 4;
        }

        public void purpleClick5Andy()
        {
            ObjectIndex = 5;
        }



        public void Update()
        {
            _QuitOnConnectionErrors();

           
            if (Frame.TrackingState != FrameTrackingState.Tracking)
            {
                const int LOST_TRACKING_SLEEP_TIMEOUT = 15;
                Screen.sleepTimeout = LOST_TRACKING_SLEEP_TIMEOUT;
                return;
            }

            Screen.sleepTimeout = SleepTimeout.NeverSleep;
            Frame.GetNewPlanes(ref m_newPlanes);

         
            for (int i = 0; i < m_newPlanes.Count; i++)
            {
                
                GameObject planeObject = Instantiate(m_trackedPlanePrefab, Vector3.zero, Quaternion.identity,
                    transform);
                planeObject.GetComponent<TrackedPlaneVisualizer>().SetTrackedPlane(m_newPlanes[i]);

                planeObject.GetComponent<Renderer>().material.SetColor("_GridColor", m_planeColors[Random.Range(0,
                    m_planeColors.Length - 1)]);
                planeObject.GetComponent<Renderer>().material.SetFloat("_UvRotation", Random.Range(0.0f, 360.0f));
            }

            
            bool showSearchingUI = true;
            Frame.GetAllPlanes(ref m_allPlanes);
            for (int i = 0; i < m_allPlanes.Count; i++)
            {
                if (m_allPlanes[i].IsValid)
                {
                    showSearchingUI = false;
                    break;
                }
            }

            m_searchingForPlaneUI.SetActive(showSearchingUI);

            Touch touch;
            if (Input.touchCount < 1 || (touch = Input.GetTouch(0)).phase != TouchPhase.Began)
            {
                return;
            }

            TrackableHit hit;
            TrackableHitFlag raycastFilter = TrackableHitFlag.PlaneWithinBounds | TrackableHitFlag.PlaneWithinPolygon;




            if (Session.Raycast(m_firstPersonCamera.ScreenPointToRay(touch.position), raycastFilter, out hit))
            {
               
                var anchor = Session.CreateAnchor(hit.Point, Quaternion.identity);



                if (ObjectIndex == 0)
                {

                }

                if (ObjectIndex == 1)
                {
                    var andyObject = Instantiate(m_andyAndroidPrefab, hit.Point, Quaternion.identity,
                    anchor.transform);
                    andyObject.tag = "markerTag";
                   
                    andyObject.transform.LookAt(m_firstPersonCamera.transform);
                    andyObject.transform.rotation = Quaternion.Euler(0.0f,
                        andyObject.transform.rotation.eulerAngles.y, andyObject.transform.rotation.z);
                    
                    andyObject.GetComponent<PlaneAttachment>().Attach(hit.Plane);
                    ObjectIndex = 0;



                }




                if (ObjectIndex == 2)
                {
                    var andyObject2 = Instantiate(m_2andyAndroidPrefab, hit.Point, Quaternion.identity,
                    anchor.transform);
                    andyObject2.tag = "markerTag";
                    
                    andyObject2.transform.LookAt(m_firstPersonCamera.transform);
                    andyObject2.transform.rotation = Quaternion.Euler(0.0f,
                        andyObject2.transform.rotation.eulerAngles.y, andyObject2.transform.rotation.z);

                 
                    andyObject2.GetComponent<PlaneAttachment>().Attach(hit.Plane);
                    ObjectIndex = 0;

                }


                if (ObjectIndex == 3)
                {
                    var andyObject3 = Instantiate(m_3andyAndroidPrefab, hit.Point, Quaternion.identity,
                    anchor.transform);
                    andyObject3.tag = "markerTag";
                    
                    andyObject3.transform.LookAt(m_firstPersonCamera.transform);
                    andyObject3.transform.rotation = Quaternion.Euler(0.0f,
                        andyObject3.transform.rotation.eulerAngles.y, andyObject3.transform.rotation.z);

                  
                    andyObject3.GetComponent<PlaneAttachment>().Attach(hit.Plane);
                    ObjectIndex = 0;

                }

                if (ObjectIndex == 4)
                {
                    var andyObject4 = Instantiate(m_4andyAndroidPrefab, hit.Point, Quaternion.identity,
                    anchor.transform);
                    andyObject4.tag = "markerTag";
                 
                    andyObject4.transform.LookAt(m_firstPersonCamera.transform);
                    andyObject4.transform.rotation = Quaternion.Euler(0.0f,
                        andyObject4.transform.rotation.eulerAngles.y, andyObject4.transform.rotation.z);

                    andyObject4.GetComponent<PlaneAttachment>().Attach(hit.Plane);
                    ObjectIndex = 0;

                }


                if (ObjectIndex == 5)
                {
                    var andyObject5 = Instantiate(m_5andyAndroidPrefab, hit.Point, Quaternion.identity,
                    anchor.transform);
                    andyObject5.tag = "markerTag";
                    
                    andyObject5.transform.LookAt(m_firstPersonCamera.transform);
                    andyObject5.transform.rotation = Quaternion.Euler(0.0f,
                        andyObject5.transform.rotation.eulerAngles.y, andyObject5.transform.rotation.z);

                    andyObject5.GetComponent<PlaneAttachment>().Attach(hit.Plane);
                    ObjectIndex = 0;

                }
            }
        }


        public void Reset1()
        {
            for (int i = 0; i < GameObject.FindGameObjectsWithTag("markerTag").Length; i++)
            {
                Destroy(GameObject.FindGameObjectsWithTag("markerTag")[i]);
            }
            ObjectIndex = 0;

        }



        private void _QuitOnConnectionErrors()
        {
           
            if (Session.ConnectionState == SessionConnectionState.DeviceNotSupported)
            {
                _ShowAndroidToastMessage("This device does not support ARCore.");
                Application.Quit();
            }
            else if (Session.ConnectionState == SessionConnectionState.UserRejectedNeededPermission)
            {
                _ShowAndroidToastMessage("Camera permission is needed to run this application.");
                Application.Quit();
            }
            else if (Session.ConnectionState == SessionConnectionState.ConnectToServiceFailed)
            {
                _ShowAndroidToastMessage("ARCore encountered a problem connecting.  Please start the app again.");
                Application.Quit();
            }
        }

        private static void _ShowAndroidToastMessage(string message)
        {
            AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
            AndroidJavaObject unityActivity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");

            if (unityActivity != null)
            {
                AndroidJavaClass toastClass = new AndroidJavaClass("android.widget.Toast");
                unityActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
                {
                    AndroidJavaObject toastObject = toastClass.CallStatic<AndroidJavaObject>("makeText", unityActivity,
                        message, 0);
                    toastObject.Call("show");
                }));
            }
        }
    }
}
