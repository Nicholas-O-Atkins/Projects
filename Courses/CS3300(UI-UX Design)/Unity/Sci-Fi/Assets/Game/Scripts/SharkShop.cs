using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SharkShop : MonoBehaviour
{
    [SerializeField]
    private AudioClip winsound;

    private void OnTriggerStay(Collider other)
    {
        if(other.tag == "Player")
        {
            if (Input.GetKeyDown(KeyCode.E))
            {
                Player player = other.GetComponent<Player>();

                if (player != null)
                {
                    if (player.hasCoin)
                    {
                        UIManager uimana = GameObject.Find("Canvas").GetComponent<UIManager>();

                        if(uimana != null)
                        {
                            player.hasCoin = false;
                            uimana.RemoveCoin();
                            AudioSource.PlayClipAtPoint(winsound, Camera.main.transform.position, 0.5f);
                            player.ShowWeapon();
                        }
                    }
                    else
                    {
                        Debug.Log("No coin here");
                    }
                }
            }
        }
    }
}
