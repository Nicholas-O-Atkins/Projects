using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    [SerializeField]
    private GameObject muzzelFlash;
    [SerializeField]
    private GameObject hitmarkerP;
    [SerializeField]
    private GameObject weapon;

    private UIManager uimana;

    [SerializeField]
    private AudioSource shooting;

    private CharacterController controller;

    private Coroutine reloader;

    private float speed = 3.5f;
    private float gravity = 9.81f;

    private int maxAmmo = 50;
    [SerializeField]
    private int currentAmmo;

    private bool reloading = false;
    private bool hasWeapon = false;
    public bool hasCoin = false;
	// Use this for initialization
	void Start ()
    {
        controller = GetComponent<CharacterController>();
        Cursor.visible = false;
        Cursor.lockState = CursorLockMode.Locked;
        currentAmmo = maxAmmo;
        uimana = GameObject.Find("Canvas").GetComponent<UIManager>();
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (Input.GetMouseButton(0) && currentAmmo > 0 && hasWeapon) // New addition
        {
            if (reloading)
            {
                StopCoroutine(reloader);
                uimana.ReloadingOff(); // new addition
                reloading = false;
            }
            Shoot();
        }
        else
        {
            muzzelFlash.SetActive(false);
            shooting.Stop();
        }

        if (Input.GetKeyDown(KeyCode.R) && !reloading && hasWeapon) // New addition
        {
            reloader = StartCoroutine(Reload());
            uimana.ReloadingOn();
        }

        if(Input.GetKeyDown(KeyCode.Escape))
        {
            Cursor.visible = true;
            Cursor.lockState = CursorLockMode.None;
        }

        calculateMovement();
    }
    
    void calculateMovement()
    {
        float horizontalInput = Input.GetAxis("Horizontal");
        float verticalInput = Input.GetAxis("Vertical");
        Vector3 direction = new Vector3(horizontalInput, 0, verticalInput);
        Vector3 velocity = direction * speed;
        velocity.y -= gravity;
        velocity = transform.TransformDirection(velocity);
        controller.Move(velocity * Time.deltaTime);
    }

    void Shoot()
    {
        muzzelFlash.SetActive(true);
        currentAmmo--;
        uimana.updateAmmo(currentAmmo);
        if (!shooting.isPlaying)
        {
            shooting.Play();
        }

        Ray rayOrigin = Camera.main.ViewportPointToRay(new Vector3(0.5f, 0.5f, 0));
        RaycastHit hitInfo;

        if (Physics.Raycast(rayOrigin, out hitInfo))
        {
            GameObject hitmarker = Instantiate(hitmarkerP, hitInfo.point, Quaternion.LookRotation(hitInfo.normal));
            Destroy(hitmarker, 1f);

            Destructable crate = hitInfo.transform.GetComponent<Destructable>();

            if (crate != null)
            {
                crate.DestroyCrate();
            }
        }
    }

    public void ShowWeapon()
    {
        weapon.SetActive(true);
        hasWeapon = true;
    }

    IEnumerator Reload()
    {
        reloading = true;
        yield return new WaitForSeconds(1.5f);
        reloading = false;
        uimana.ReloadingOff(); // new addition
        currentAmmo = maxAmmo;
        uimana.updateAmmo(currentAmmo);
    }
}
