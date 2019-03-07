using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    private float _speed = 2.0f;
    [SerializeField]
    private GameObject deathAnim;
    public int level;

    public GameObject explo; //var for the explosion object
    private GameManager game;
    private UIManager uiMana;
    [SerializeField]
    private AudioClip audioSource;

	// Use this for initialization
	void Start ()
    {
        uiMana = GameObject.Find("Canvas").GetComponent<UIManager>();
        game = GameObject.Find("GameManager").GetComponent<GameManager>(); //Added to get the gameover value
	}
	
	// Update is called once per frame
	void Update ()
    {
        transform.Translate(Vector3.down * _speed * Time.deltaTime);
        //Added to remove the enemies after the player dies
        if (game.gameOver)
        {
            Destroy(this.gameObject);
            this.gameObject.transform.localScale = new Vector3(1f, 1f, 1f);
            //To make sure the level is reset, precausionary 
            level = 0;
        }
        else if(transform.position.y <= -7)
        {
            transform.position = new Vector3(Random.Range(-8f, 8f), 7, 0);
        }
        //Added to adjust the size and speed of the enemies, tested so that the changes are noticable
        if (level != uiMana.level)
        {
            level = uiMana.level;
            _speed = 2.0f + 0.2f * (float)level;
            transform.localScale = new Vector3(1.0f - 0.05f * (float)level, 1.0f - 0.05f * (float)level, 1.0f - 0.05f * (float)level);
        }
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        Player player = other.GetComponent<Player>();
        Laser laser = other.GetComponent<Laser>();

        if (player != null)
        {
            player.Hit();
            uiMana.UpdateScore();
            explo = Instantiate(deathAnim, transform.position, Quaternion.identity);
            explo.transform.localScale = new Vector3(1.0f - 0.05f * (float)uiMana.level, 1.0f - 0.05f * (float)uiMana.level, 1.0f - 0.05f * (float)uiMana.level); //Adjust the size of the explosion
            AudioSource.PlayClipAtPoint(audioSource, Camera.main.transform.position);
            Destroy(this.gameObject);
        }

        if(laser != null)
        {
            Destroy(other.gameObject);
            uiMana.UpdateScore();
            explo = Instantiate(deathAnim, transform.position, Quaternion.identity);
            explo.transform.localScale = new Vector3(1.0f - 0.05f * (float)uiMana.level, 1.0f - 0.05f * (float)uiMana.level, 1.0f - 0.05f * (float)uiMana.level); //Adjust the size of the explosion
            AudioSource.PlayClipAtPoint(audioSource, Camera.main.transform.position);
            Destroy(this.gameObject);
        }
    }
}
