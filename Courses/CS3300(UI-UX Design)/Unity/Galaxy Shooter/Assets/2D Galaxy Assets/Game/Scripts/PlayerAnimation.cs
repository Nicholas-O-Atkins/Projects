using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAnimation : MonoBehaviour
{
    private Animator anim;
	// Use this for initialization
	void Start ()
    {
        anim = GetComponent<Animator>();
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            anim.SetBool("Turn_Left", true);
            anim.SetBool("Turn_Right", false);
        }
        if (Input.GetKeyDown(KeyCode.D))
        {
            anim.SetBool("Turn_Right", true);
            anim.SetBool("Turn_Left", false);
        }
        if(Input.GetKeyUp(KeyCode.D) || Input.GetKeyUp(KeyCode.A))
        {
            anim.SetBool("Turn_Right", false);
            anim.SetBool("Turn_Left", false);
        }
	}
}
