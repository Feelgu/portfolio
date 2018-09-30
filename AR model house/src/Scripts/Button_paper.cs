using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Button_paper : MonoBehaviour {

    public void ScenechangeToPaper()
    {
        SceneManager.LoadScene("AR_paper");
    }

    public void ScenechangeToMain()
    {
        SceneManager.LoadScene("Menu");
    }

    public void ScenechangeToPortal()
    {
        SceneManager.LoadScene("AR_portal");
    }

    public void ScenechangeToOutlook()
    {
        SceneManager.LoadScene("AR_outlook");
    }

    public void Out()
    {
        Application.Quit();
    }
}
