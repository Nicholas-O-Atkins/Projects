using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnManager : MonoBehaviour
{
    [SerializeField]
    private GameObject enemyship;
    [SerializeField]
    private GameObject[] powerups;

    private GameManager gameMana;

    // Use this for initialization
    void Start()
    {
        gameMana = GameObject.Find("GameManager").GetComponent<GameManager>();

        StartCoroutine(EnemySpawner());
        StartCoroutine(PowerupSpawner());
    }

    public void StartSpawning()
    {
        StartCoroutine(EnemySpawner());
        StartCoroutine(PowerupSpawner());
    }

    //create coroutine to spawn the enemy every 5 secs
    IEnumerator EnemySpawner()
    {
        while(!gameMana.gameOver)
        {
            Instantiate(enemyship, new Vector3(Random.Range(-8f, 8f), 7, 0), Quaternion.identity);
            yield return new WaitForSeconds(2.0f);
        }
    }

    IEnumerator PowerupSpawner()
    {

        while(!gameMana.gameOver)
        {
            int randomPowerup = Random.Range(0, 3);
            Instantiate(powerups[randomPowerup], new Vector3(Random.Range(-8, 8), 8, 0), Quaternion.identity);
            yield return new WaitForSeconds(10.0f);
        }

    }
}
