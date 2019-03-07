using System.Collections;
using System.Collections.Generic;
using UnityEngine;
/*This is a New class created for the purpose of changing the pitch of the 
 * 
 * 
 */
public class PitchControl : MonoBehaviour
{
    private UIManager uiMana;
    private AudioSource track;
    private float levelfloat;
	// Use this for initialization
	void Start ()
    {
        uiMana = GameObject.Find("Canvas").GetComponent<UIManager>();
        track = GetComponent<AudioSource>();
	}
	
	public void UpdatePitch ()
    {
        levelfloat = ((float)uiMana.level) / 10;

        track.pitch = 1 + levelfloat;
	}
    
    public void ResetPitch()
    {
        track.pitch = 1;
    }
}
