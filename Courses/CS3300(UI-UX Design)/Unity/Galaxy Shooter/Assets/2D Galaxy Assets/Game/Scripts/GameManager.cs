using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public bool gameOver = true;
    public GameObject player;

    public UIManager uimana;

    private void Start()
    {
        uimana = GameObject.Find("Canvas").GetComponent<UIManager>();
    }

    private void Update()
    {
        if (gameOver)
        {
            if (Input.GetKeyDown(KeyCode.Space))
            {
                Instantiate(player, Vector3.zero, Quaternion.identity);
                gameOver = false;
                uimana.HideTitle();
            }
        }
    }

}
