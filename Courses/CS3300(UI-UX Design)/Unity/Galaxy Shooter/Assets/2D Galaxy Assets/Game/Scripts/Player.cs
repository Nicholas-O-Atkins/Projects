using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour {

    public bool canTripleS = false;
    public bool hasSpeed = false;
    public bool hasShield = false;

    [SerializeField]
    public int health = 3;
    [SerializeField]
    private GameObject _laserPrefab;
    [SerializeField]
    private GameObject deathAni;
    [SerializeField]
    private GameObject shieldObj;
    [SerializeField]
    private GameObject[] engines;

    [SerializeField]
    private float _fireRate = 0.25f;
    private float _canFire = 0.0f;

    [SerializeField]
    private float _speed = 5.0f;

    private UIManager uiMana;
    private GameManager gameMana;
    private SpawnManager spawnMana;
    private AudioSource audioSource;

    private int hitcount = 0;

    // Use this for initialization
    void Start ()
    {
        transform.position = new Vector3(0, 0, 0);

        uiMana = GameObject.Find("Canvas").GetComponent<UIManager>();

        spawnMana = GameObject.Find("Spawn_Manager").GetComponent<SpawnManager>();

        if (uiMana != null)
        {
            uiMana.UpdateLives(health);
        }

        gameMana = GameObject.Find("GameManager").GetComponent<GameManager>();

        if(spawnMana != null)
        {
            spawnMana.StartSpawning();
        }

        audioSource = GetComponent<AudioSource>();

        hitcount = 0;
    }
	
	// Update is called once per frame
	void Update ()
    {

        Movement();

        if (Input.GetKeyDown(KeyCode.Space))
        {
            Shoot();

        }

	}

    //movement
    private void Movement()
    {
        float horizontalInput = Input.GetAxis("Horizontal");
        float verticalInput = Input.GetAxis("Vertical");

        if (hasSpeed)
        {
            transform.Translate(Vector3.right * horizontalInput * _speed * 2 * Time.deltaTime);
            transform.Translate(Vector3.up * verticalInput * _speed * 2 * Time.deltaTime);
        }
        else
        {
            transform.Translate(Vector3.right * horizontalInput * _speed * Time.deltaTime);
            transform.Translate(Vector3.up * verticalInput * _speed * Time.deltaTime);
        }

        

        if (transform.position.y >= 0)
        {
            transform.position = new Vector3(transform.position.x, 0, 0);
        }
        else if (transform.position.y < -4.2f)
        {
            transform.position = new Vector3(transform.position.x, -4.2f, 0);
        }
        if (transform.position.x < -9.6f)
        {
            transform.position = new Vector3(9.5f, transform.position.y, 0);
        }
        else if (transform.position.x > 9.6f)
        {
            transform.position = new Vector3(-9.5f, transform.position.y, 0);
        }
    }

    private void Shoot()
    {
        if (Time.time > _canFire)
        {
            audioSource.Play();
            Instantiate(_laserPrefab, transform.position + new Vector3(0, 1.8f, 0), Quaternion.identity);
            _canFire = Time.time + _fireRate;
            if (canTripleS)
            {
                Instantiate(_laserPrefab, transform.position + new Vector3(0.56f, 0.3f, 0), Quaternion.identity);
                Instantiate(_laserPrefab, transform.position + new Vector3(-0.56f, 0.3f, 0), Quaternion.identity);
            }
        }
    }

    public void checkHealth()
    {
        if(health < 1)
        {
            //Game over
            Instantiate(deathAni, transform.position, Quaternion.identity);
            gameMana.gameOver = true;
            uiMana.ShowTitle();
            Destroy(this.gameObject);
        }
    }

    public void Hit()
    {
        if(hasShield)
        {
            hasShield = false;
            shieldObj.SetActive(false);
        }
        else
        {
            health--;
            hitcount++;
            if(hitcount == 1)
            {
                engines[0].SetActive(true);
            }
            else if(hitcount == 2)
            {
                engines[1].SetActive(true);
            }
            uiMana.UpdateLives(health);
        }   
        checkHealth();
    }

    public void TripleShotPowerShotOn()
    {
        canTripleS = true;
        StartCoroutine(TripleShotPowerDown());
    }

    public IEnumerator TripleShotPowerDown()
    {
        yield return new WaitForSeconds(5.0f);
        canTripleS = false;
    }
    public void SpeedPowerOn()
    {
        hasSpeed = true;
        StartCoroutine(SpeedPowerDown());
    }
    public IEnumerator SpeedPowerDown()
    {
        yield return new WaitForSeconds(5.0f);
        hasSpeed = false;
    }
    public void ShieldPowerOn()
    {
        hasShield = true;
        shieldObj.SetActive(true);
    }

}
