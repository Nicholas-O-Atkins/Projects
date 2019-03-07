using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Powerup : MonoBehaviour {
    [SerializeField]
    private float _speed = 1.0f;
    [SerializeField]
    private int powerupID; //0 = triple shot 1 = speed 2 = shield
    [SerializeField]
    private AudioClip clip;

	// Use this for initialization
	void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        transform.Translate(Vector3.down * _speed * Time.deltaTime);
        if(transform.position.y <= -7)
        {
            Destroy(this.gameObject);
        }
	}

    private void OnTriggerEnter2D(Collider2D other)
    {
        if(other.tag == "Player")
        {
            Player player = other.GetComponent<Player>();

            if(player != null)
            {
                if(powerupID == 0)
                {
                    player.TripleShotPowerShotOn();
                }
                else if(powerupID == 1)
                {
                    player.SpeedPowerOn();
                }
                else if(powerupID == 2)
                {
                    player.ShieldPowerOn();
                }

                AudioSource.PlayClipAtPoint(clip, Camera.main.transform.position);

                Destroy(this.gameObject);
            }
        }

    }
}
